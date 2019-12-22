#ifndef CPPREQUESTS_CONST_H
#define CPPREQUESTS_CONST_H

#include <curl/curl.h>

#include "requests/http.h"
#include "requests/structures.h"

namespace crq {

    namespace curl {

        const std::map<CURLcode, std::pair<std::string, std::string>> LIBCURL_CODE = {
                {CURLE_OK,                   {
                                                     "CURLE_OK",
                                                     "All fine. Proceed as usual."
                                             }},
                {CURLE_UNSUPPORTED_PROTOCOL, {
                                                     "CURLE_UNSUPPORTED_PROTOCOL",
                                                     "The URL you passed to libcurl used a protocol "
                                                     "that this libcurl does not support. The support might be "
                                                     "a compile-time option that you didn't use, it can be a "
                                                     "misspelled protocol string or just a protocol libcurl has no code for."
                                             }},
                {CURLE_FAILED_INIT,          {
                                                     "CURLE_FAILED_INIT",
                                                     "Very early initialization code failed. This is "
                                                     "likely to be an internal error or problem, or a resource "
                                                     "problem where something fundamental couldn't get done at init time."
                                             }},
//            {CURLE_URL_MALFORMAT, "CURLE_URL_MALFORMAT"},
//            {CURLE_NOT_BUILT_IN, "CURLE_NOT_BUILT_IN"},
//            {CURLE_COULDNT_RESOLVE_PROXY, "CURLE_COULDNT_RESOLVE_PROXY"},
//            {CURLE_COULDNT_RESOLVE_HOST, "CURLE_COULDNT_RESOLVE_HOST"},
        };

        const std::map<CURLUcode, std::pair<std::string, std::string>> LIBCURL_URL_CODE = {
                {CURLUE_OK, {"CURLUE_OK", "CURLUE_OK"}},
                {CURLUE_BAD_HANDLE, {"CURLUE_BAD_HANDLE", "CURLUE_BAD_HANDLE"}},
                {CURLUE_BAD_PARTPOINTER, {"CURLUE_BAD_PARTPOINTER", "CURLUE_BAD_PARTPOINTER"}},
                {CURLUE_MALFORMED_INPUT, {"CURLUE_MALFORMED_INPUT", "CURLUE_MALFORMED_INPUT"}},
                {CURLUE_BAD_PORT_NUMBER, {"CURLUE_BAD_PORT_NUMBER", "CURLUE_BAD_PORT_NUMBER"}},
                {CURLUE_UNSUPPORTED_SCHEME, {"CURLUE_UNSUPPORTED_SCHEME", "CURLUE_UNSUPPORTED_SCHEME"}},
                {CURLUE_URLDECODE, {"CURLUE_URLDECODE", "CURLUE_URLDECODE"}},
                {CURLUE_OUT_OF_MEMORY, {"CURLUE_OUT_OF_MEMORY", "CURLUE_OUT_OF_MEMORY"}},
                {CURLUE_USER_NOT_ALLOWED, {"CURLUE_USER_NOT_ALLOWED", "CURLUE_USER_NOT_ALLOWED"}},
                {CURLUE_UNKNOWN_PART, {"CURLUE_UNKNOWN_PART", "CURLUE_UNKNOWN_PART"}},
                {CURLUE_NO_SCHEME, {"CURLUE_NO_SCHEME", "CURLUE_NO_SCHEME"}},
                {CURLUE_NO_USER, {"CURLUE_NO_USER", "CURLUE_NO_USER"}},
                {CURLUE_NO_PASSWORD, {"CURLUE_NO_PASSWORD", "CURLUE_NO_PASSWORD"}},
                {CURLUE_NO_OPTIONS, {"CURLUE_NO_OPTIONS", "CURLUE_NO_OPTIONS"}},
                {CURLUE_NO_HOST, {"CURLUE_NO_HOST", "CURLUE_NO_HOST"}},
                {CURLUE_NO_PORT, {"CURLUE_NO_PORT", "CURLUE_NO_PORT"}},
                {CURLUE_NO_QUERY, {"CURLUE_NO_QUERY", "CURLUE_NO_QUERY"}},
                {CURLUE_NO_FRAGMENT, {"CURLUE_NO_FRAGMENT", "CURLUE_NO_FRAGMENT"}}
        };

        const std::map<http::HTTP_REQUEST_TYPE, std::string> LIBCURL_HTTP_VERB{
                {http::GET,  "GET"},
                {http::POST, "POST"},
        };
    }

    inline std::string default_user_agent(const std::string &name = "cpp-requests") {
        return fmt::format("{0}/{1}", name, "1.0.0");
    }

    inline crq::HeaderMap default_headers() {
        return {
                {"User-Agent",      default_user_agent()},
                {"Accept-Encoding", "gzip, deflate"},
                {"Accept",          "*/*"},
                {"Accept-Language", "zh-CN,zh;q=0.8"},
                {"Connection",      "keep-alive"}
        };
    }

    inline crq::HeaderMap merge_headers(const crq::HeaderMap &new_header,
                                        const crq::HeaderMap &inner_header) {
        auto buf = HeaderMap(inner_header.begin(), inner_header.end());

        for (auto &item: new_header) {
            buf[item.first] = item.second;
        }

        return buf;
    }
}


#endif //CPPREQUESTS_CONST_H
