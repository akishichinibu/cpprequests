#include <string>
#include <iostream>

#include "requests/extends/_string.h"

int main() {
    auto s = std::string {"happy day"};
    std::cout << s.size() << std::endl;

    std::cout << (s | crq::string::upper()) << std::endl;

    std::cout << (s | crq::string::upper() | crq::string::strip("hd")) << std::endl;
}
