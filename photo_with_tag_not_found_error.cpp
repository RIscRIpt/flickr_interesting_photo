#include "photo_with_tag_not_found_error.h"

using namespace std::string_literals;

photo_with_tag_not_found_error::photo_with_tag_not_found_error(std::string const &tag)
    : std::runtime_error("couldn't find photo with tag `"s + tag + "'"s)
{}

