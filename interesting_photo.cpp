#include "interesting_photo.h"
#include "photo_with_tag_not_found_error.h"

#include <unordered_set>

#include <cpprest/filestream.h>

using namespace std::placeholders;

std::string const FLICKR_API_HOST = "https://api.flickr.com";
std::string const FLICKR_API_URI  = "/services/rest/";

interesting_photo::interesting_photo(std::string const &api_key)
    : api_key_(api_key)
{}

pplx::task<web::json::value> interesting_photo::extract_result(web::http::http_response const &response) const {
    if (response.status_code() != web::http::status_codes::OK) {
        return pplx::task_from_result(web::json::value());
    }
    return response.extract_json();
}

utility::string_t interesting_photo::get_photo_with_tag(pplx::task<web::json::value> task, utility::string_t const &tag) const {
    auto photos = task
        .get()
        .at("photos")
        .at("photo")
        .as_array();
    for (auto photo : photos) {
        utility::istringstream_t iss(photo.at("tags").as_string());
        std::unordered_set<utility::string_t> tags{std::istream_iterator<utility::string_t>{iss},
                                                   std::istream_iterator<utility::string_t>{}};
        if (tags.find(tag) != tags.end()) {
            return photo.at("id").as_string();
        }
    }
    throw photo_with_tag_not_found_error(tag);
}

pplx::task<web::json::value> interesting_photo::get_photo_urls(utility::string_t const &id) const {
    web::http::client::http_client client(FLICKR_API_HOST);
    web::uri_builder url(FLICKR_API_URI);
    url.append_query("method",  "flickr.photos.getSizes");
    url.append_query("api_key", api_key_);
    url.append_query("format", "json");
    url.append_query("nojsoncallback", "1");
    url.append_query("photo_id", id);
    return client
        .request(web::http::methods::GET, url.to_string())
        .then(std::bind(&interesting_photo::extract_result, this, _1));
}

utility::string_t interesting_photo::get_photo_url_with_max_size(pplx::task<web::json::value> task) const {
    return task
        .get()
        .at("sizes")
        .at("size")
        .as_array()
        .rbegin()
        ->at("source")
        .as_string();
}

utility::string_t interesting_photo::get(utility::string_t const &tag) const {
    web::http::client::http_client client(FLICKR_API_HOST);
    web::uri_builder url(FLICKR_API_URI);
    url.append_query("method",  "flickr.interestingness.getList");
    url.append_query("api_key", api_key_);
    url.append_query("format", "json");
    url.append_query("nojsoncallback", "1");
    url.append_query("page", "1");
    url.append_query("per_page", "500");
    url.append_query("extras", "tags");
    return client
        .request(web::http::methods::GET, url.to_string())
        .then(std::bind(&interesting_photo::extract_result, this, _1))
        .then(std::bind(&interesting_photo::get_photo_with_tag, this, _1, tag))
        .then(std::bind(&interesting_photo::get_photo_urls, this, _1))
        .then(std::bind(&interesting_photo::get_photo_url_with_max_size, this, _1))
        .get();
}

utility::string_t interesting_photo::all() const {
    web::http::client::http_client client(FLICKR_API_HOST);
    web::uri_builder url(FLICKR_API_URI);
    url.append_query("method",  "flickr.interestingness.getList");
    url.append_query("api_key", api_key_);
    url.append_query("format", "json");
    url.append_query("nojsoncallback", "1");
    url.append_query("page", "1");
    url.append_query("per_page", "500");
    url.append_query("extras", "tags");
    return client
        .request(web::http::methods::GET, url.to_string())
        .then(std::bind(&interesting_photo::extract_result, this, _1))
        .get()
        .serialize();
}

