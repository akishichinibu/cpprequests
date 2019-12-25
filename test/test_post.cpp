#include <gtest/gtest.h>

#include "requests/models/session.h"


TEST(TEST_CPPREQUESTS, TEST_HTTP_POST_SIMPLE) {
    const std::string url = "http://jsonplaceholder.typicode.com/posts";

    auto req = crq::Request("POST", url)
            .timeout(300000)
            .allow_redirects(true)
            .json({{"title", "foo"}, {"body", "bar"}, {"userId", 1}});

    auto session = crq::Session();
    auto res = session.send(req);

    ASSERT_EQ(res.status_code() / 100, 2);
    ASSERT_EQ(res.status_code(), crq::http::HTTP_STATUS_CODE::CREATED);

    const auto body = res.json();
    ASSERT_EQ(body["id"], 101);
}


TEST(TEST_CPPREQUESTS, TEST_HTTP_POST_TIMEOUT) {
    const std::string url = "http://jsonplaceholder.typicode.com/posts";

    auto req = crq::Request("POST", url)
            .timeout(1)
            .allow_redirects(true)
            .json({{"title", "foo"}, {"body", "bar"}, {"userId", 1}});

    auto session = crq::Session();
    ASSERT_THROW(session.send(req), std::runtime_error);
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
