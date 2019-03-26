#include "interesting_photo_service.h"
#include "photo_with_tag_not_found_error.h"

#include <thread>

using namespace std::placeholders;

namespace web::http::listener {
    using namespace web::http::experimental::listener;
}

interesting_photo_service::interesting_photo_service(std::string const &flickr_api_key)
    : interesting_photo_(flickr_api_key)
{}

int interesting_photo_service::run() {
    web::http::listener::http_listener listener("http://0.0.0.0:80/interestingPhoto");
    listener.support(
            web::http::methods::GET,
            std::bind(&interesting_photo_service::handle_get, this, _1)
            );
    try {
        listener
            .open()
            .then([&listener]() {
                std::cout << "listening to " << listener.uri().to_string() << '\n';
            })
            .wait();
        std::this_thread::sleep_until(std::chrono::time_point<std::chrono::system_clock>::max());
        return 0;
    } catch (std::exception const &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}

void interesting_photo_service::handle_get(web::http::http_request request) {
    std::string tag = request.relative_uri().to_string().erase(0, 1);
    web::http::http_response response;
    try {
        if (tag.empty()) {
            response.set_status_code(web::http::status_codes::OK);
            response.set_body(interesting_photo_.all());
            request.reply(response);
            return;
        }

        auto url = interesting_photo_.get(tag);
        if (url.empty()) {
            response.set_status_code(web::http::status_codes::NotFound);
            response.set_body("not found");
            std::cout << '`' << tag << "` -> not found" << '\n';
            request.reply(response);
            return;
        }

        response.set_status_code(web::http::status_codes::TemporaryRedirect);
        response.headers().add("Location", url);
        response.set_body(url);
        std::cout << '`' << tag << "` -> " << url << '\n';
        request.reply(response);
    } catch (photo_with_tag_not_found_error const &e) {
        response.set_status_code(web::http::status_codes::NotFound);
        response.set_body("not found");
        std::cout << '`' << tag << "` -> " << e.what() << '\n';
        request.reply(response);
    } catch (std::exception const &e) {
        response.set_status_code(web::http::status_codes::InternalError);
        response.set_body("internal server error");
        std::cout << '`' << tag << "` -> " << e.what() << '\n';
        request.reply(response);
    }
}

