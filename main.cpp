#include "interesting_photo_service.h"

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
    std::vector<std::string> args(argv, argv + argc);

    if (args.size() != 2) {
        std::cerr << "usage: " << args[0] << " <api_key>\n ";
        return -1;
    }

    auto const &api_key = args[1];
    interesting_photo_service ips(api_key);
    return ips.run();
}

