#ifndef CPPREQUESTS_LIBCURL_H
#define CPPREQUESTS_LIBCURL_H

#include <atomic>
#include <map>
#include <iostream>
#include <mutex>
#include <functional>
#include <curl/curl.h>

#include <nlohmann/json.hpp>

#include "requests/utils.h"

namespace crq {

    using CURLptrType = std::unique_ptr<CURL, decltype(curl_easy_cleanup)*>;

    using CURLsListType = std::unique_ptr<curl_slist, decltype(curl_slist_free_all)*>;

    class libcurlStreamBuffer {

    private:
        CURLptrType request_handler;

        HeaderMap header_buffer;
        std::string body_buffer;

        bool consumed = false;

    public:
        explicit libcurlStreamBuffer(CURLptrType request_handler):
        request_handler(std::move(request_handler)) {

            std::cout << ">?>?" << this->request_handler.get() << std::endl;
        }

        static std::size_t write_callback(void *contents,
                std::size_t size,
                std::size_t nmemb,
                void *userdata) {

            auto convert_userdata = (std::string*)(userdata);
            const std::size_t content_length = size * nmemb;

            convert_userdata->append((char*)contents, content_length);
            return content_length;
        }

        CURLptrType& request() {
            return this->request_handler;
        }

        HeaderMap& header() {
            return this->header_buffer;
        }

        std::string& body() {
            return this->body_buffer;
        }
    };


    typedef struct {
        std::string url;

        int status_code;
        libcurlStreamBuffer stream_buffer;

        long elapsed;
    } RequestReturn;


    class libcurlAdapter {

    private:
        static std::atomic<bool> global_init_status;
        static std::atomic<bool> global_clean_status;
        static std::mutex global_execute;
        static std::lock_guard<std::mutex> global_execute_lock;

        static void libcurl_global_init();

        static void libcurl_global_clean();

        static CURLptrType init_request();

        static std::size_t header_callback(void *contents, std::size_t size, std::size_t nmemb, void *userdata);

    public:
        enum {
            GET,
            POST
        } RequestType;

        static void init();

        static void close();

        int timeout;
        bool allow_redirects;
        bool stream;

        explicit libcurlAdapter(int timeout = 3,
                bool allow_redirects = true,
                bool stream = false);

        RequestReturn get(std::string &url,
                const HeaderMap &headers = default_headers());

        RequestReturn post(std::string &url,
                const std::string& params,
                const HeaderMap &headers = default_headers());

        RequestReturn post(std::string &url,
                const nlohmann::json& params,
                const HeaderMap &headers = default_headers());
    };


    const std::map<int, std::pair<std::string, std::string>> LIBCURL_CODE = {
            {CURLE_OK, {
                "CURLE_OK",
                "All fine. Proceed as usual."
            }},
            {CURLE_UNSUPPORTED_PROTOCOL, {
                "CURLE_UNSUPPORTED_PROTOCOL",
                "The URL you passed to libcurl used a protocol that this libcurl does not support. "
                "The support might be a compile-time option that you didn't use, "
                "it can be a misspelled protocol string or just a protocol libcurl has no code for."
            }},
            {CURLE_FAILED_INIT, {
                "CURLE_FAILED_INIT",
                "Very early initialization code failed. "
                "This is likely to be an internal error or problem, "
                "or a resource problem where something fundamental couldn't get done at init time."
            }},
//            {CURLE_URL_MALFORMAT, "CURLE_URL_MALFORMAT"},
//            {CURLE_NOT_BUILT_IN, "CURLE_NOT_BUILT_IN"},
//            {CURLE_COULDNT_RESOLVE_PROXY, "CURLE_COULDNT_RESOLVE_PROXY"},
//            {CURLE_COULDNT_RESOLVE_HOST, "CURLE_COULDNT_RESOLVE_HOST"},
    };
}

#endif //CPPREQUESTS_LIBCURL_H
