//#include <gtest/gtest.h>
//
//#include "requests/models/session.h"
//
//TEST(TEST_CPPREQUESTS, TEST_HTTP_GET_1) {
//    const std::string url = "http://jsonplaceholder.typicode.com/todos/1";
//
//    auto req = crq::Request(crq::http::GET, url);
//
//    auto session = crq::Session();
//    auto res = session.send(req);
//
//    ASSERT_EQ(res.status_code(), crq::http::OK);
//
//    const auto body = res.json();
//
//    ASSERT_EQ(body["userId"], 1);
//    ASSERT_EQ(body["id"], 1);
//    ASSERT_EQ(body["title"], "delectus aut autem");
//    ASSERT_EQ(body["completed"], false);
//
//    for (auto& item: res.headers()) {
//        std::cout << item.first << ": " << item.second << std::endl;
//    }
//}
//
//
//TEST(TEST_CPPREQUESTS, TEST_HTTP_GET_2) {
//    const std::string url = "http://jsonplaceholder.typicode.com/comments";
//
//    auto req = crq::Request(crq::http::GET, url)
//            .params_({{"postId", "1"}});
//
//    auto session = crq::Session();
//    auto res = session.send(req);
//
//    ASSERT_EQ(res.url(),
//            "http://jsonplaceholder.typicode.com/comments?postId=1");
//
//    ASSERT_EQ(res.status_code(), crq::http::OK);
//
//    const auto body = res.json();
//    ASSERT_EQ(body.size(), 5);
//
//    for (auto& item: res.headers()) {
//        std::cout << item.first << ": " << item.second << std::endl;
//    }
//}
//
int main() {
//    ::testing::InitGoogleTest();

//    return RUN_ALL_TESTS();
}
