#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <gzip/decompress.hpp>

#include "requests/backend/libcurl.h"
#include "requests/api.h"
#include "requests/models/response.h"

int main() {
    std::string url = "http://www.google.com";
    auto r = crq::get(url);
    return 0;
}
