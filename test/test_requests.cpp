#include <gtest/gtest.h>

#include "requests/models/session.h"


TEST(TEST_CPPREQUESTS, TEST_GET_HTTP) {
    const std::string url = "http://jsonplaceholder.typicode.com/todos/1";

    auto req = crq::Request(crq::http::GET,
            url,
            true,
            false,
            true,
                            false);

    auto session = crq::Session();

    auto res = session.send(req);

    std::cout << res.status_code() << std::endl;

    std::cout << res.elapsed() << std::endl;
    std::cout << res.json() << std::endl;

    for (auto& item: res.headers()) {
        std::cout << item.first << ": " << item.second << std::endl;
    }
}

int main() {
    ::testing::InitGoogleTest();

    return RUN_ALL_TESTS();
}
