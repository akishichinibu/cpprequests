#include <gtest/gtest.h>

#include "requests/models/session.h"


TEST(TEST_CPPREQUESTS, TEST_HTTP_POST_1) {
    const std::string url = "http://jsonplaceholder.typicode.com/posts";

    auto req = crq::Request(crq::http::POST, url);

    auto session = crq::Session();
    auto res = session.send(req);

    ASSERT_EQ(res.status_code() / 100, 2);
    ASSERT_EQ(res.status_code(), crq::http::HTTP_STATUS_CODE::CREATED);

    const auto body = res.json();

    ASSERT_EQ(body["id"], 101);
}

TEST(TEST_CPPREQUESTS, TEST_HTTP_POST_2) {
    const std::string url = "http://jsonplaceholder.typicode.com/posts";

    auto req = crq::Request(crq::http::POST,
            url,
            {{"Content-type", "application/json; charset=UTF-8"}})
                    .json_({{"title", "foo"}, {"body", "bar"}, {"userId", 1}});

    for (auto& item: req.headers()) {
        std::cout << item.first << ": " << item.second << std::endl;
    }

    std::cout << req.body() << std::endl;


    auto session = crq::Session();
    auto res = session.send(req);

    ASSERT_EQ(res.status_code() / 100, 2);
    ASSERT_EQ(res.status_code(), crq::http::HTTP_STATUS_CODE::CREATED);

//    const auto body = res.json();

//    std::cout << body.dump(4) << std::endl;

    std::cout << res.content() << std::endl;

    for (auto& item: res.headers()) {
        std::cout << item.first << ": " << item.second << std::endl;
    }
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
