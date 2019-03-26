#pragma once

#include <stdexcept>
#include <string>

class photo_with_tag_not_found_error : public std::runtime_error {
public:
    photo_with_tag_not_found_error(std::string const &tag);
};

