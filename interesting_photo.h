#pragma once

#include <cpprest/http_client.h>

class interesting_photo {
public:
    interesting_photo(std::string const &api_key);
    utility::string_t get(utility::string_t const &tag) const;
    utility::string_t all() const;

private:
    pplx::task<web::json::value> extract_result(web::http::http_response const &response) const;
    utility::string_t get_photo_with_tag(pplx::task<web::json::value> task, utility::string_t const &tag) const;
    pplx::task<web::json::value> get_photo_urls(utility::string_t const &id) const;
    utility::string_t get_photo_url_with_max_size(pplx::task<web::json::value> task) const;

    std::string const api_key_;
};

