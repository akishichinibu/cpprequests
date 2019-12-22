#include <gtest/gtest.h>

#include "requests/models/url.h"


TEST(TEST_CPPREQUESTS, TEST_INVALID_URL) {

    ASSERT_THROW(crq::URL{"hiwpefhipowhefopw"}, std::runtime_error);

    ASSERT_THROW(crq::URL{"localhost:3128"}, std::runtime_error);

    ASSERT_THROW(crq::URL{"localhost.localdomain:3128/"}, std::runtime_error);

    ASSERT_THROW(crq::URL{"10.122.1.1:3128/"}, std::runtime_error);

    ASSERT_THROW(crq::URL{"http://"}, std::runtime_error);
}

TEST(TEST_CPPREQUESTS, TEST_VALID_URL_1) {
    const auto url = "http://jsonplaceholder.typicode.com/todos/1";

    crq::URL s = crq::URL{url};

    ASSERT_EQ(s.scheme(), "http");

    ASSERT_EQ(s.host(), "jsonplaceholder.typicode.com");

    ASSERT_EQ(s.port(), 0);

    ASSERT_EQ(s.path(), "/todos/1");
}

TEST(TEST_CPPREQUESTS, TEST_VALID_URL_2) {
    const auto url = "HttP://aaa.bbb.ccc:12345?key=value";

    crq::URL s = crq::URL{url};

    ASSERT_EQ(s.scheme(), "http");

    ASSERT_EQ(s.host(), "aaa.bbb.ccc");

    ASSERT_EQ(s.port(), 12345);

    ASSERT_EQ(s.path(), "");

    ASSERT_EQ(s.raw_query(), "key=value");

    ASSERT_EQ(s.fragment(), "");
}

TEST(TEST_CPPREQUESTS, TEST_FULL_URL) {
    const auto url = "https://aaa.bbb.ccc:1234/ddd/1?key1=value1&key2=value2#here";

    crq::URL s = crq::URL{url};

    ASSERT_EQ(s.scheme(), "https");

    ASSERT_EQ(s.host(), "aaa.bbb.ccc");

    ASSERT_EQ(s.port(), 1234);

    ASSERT_EQ(s.path(), "/ddd/1");

    ASSERT_EQ(s.raw_query(), "key1=value1&key2=value2");

    ASSERT_EQ(s.fragment(), "here");

    const auto qs = s.params();

    ASSERT_EQ(qs.size(), 2);

    ASSERT_EQ(qs.at("key1"), "value1");

    ASSERT_EQ(qs.at("key2"), "value2");
}


int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
