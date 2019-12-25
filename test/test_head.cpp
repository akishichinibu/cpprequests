#include <gtest/gtest.h>

#include "requests/models/session.h"


TEST(TEST_CPPREQUESTS, TEST_HTTP_HEAD_1) {
    const std::string url = "http://jsonplaceholder.typicode.com/todos/1";

    auto kk = crq::URL{url};
    auto req = crq::Request("HEAD", url);

    auto session = crq::Session();
    auto res = session.send(req);

    ASSERT_EQ(res.status_code(), crq::http::OK);
    ASSERT_TRUE(res.content().empty());

    for (auto& item: res.headers()) {
        std::cout << item.first << ": " << item.second << std::endl;
    }
}


int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
