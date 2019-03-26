#pragma once

#include "interesting_photo.h"

#include <cpprest/http_listener.h>

#include <string>

class interesting_photo_service {
public:
    interesting_photo_service(std::string const &flickr_api_key);
    int run();

private:
    void handle_get(web::http::http_request request);

    interesting_photo interesting_photo_;
};

