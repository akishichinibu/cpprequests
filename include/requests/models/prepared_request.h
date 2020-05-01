#ifndef CPPREQUESTS_PREPARED_REQUEST_H
#define CPPREQUESTS_PREPARED_REQUEST_H

#include <memory>
#include <atomic>
#include <mutex>
#include <curl/curl.h>

#include "requests/const.h"
#include "requests/models/request.h"
#include "requests/models/stream.h"
#include "requests/models/url.h"


namespace crq {

    using CURLptrType = std::unique_ptr<CURL, decltype(curl_easy_cleanup)*>;

    using CURLsListType = std::unique_ptr<curl_slist, decltype(curl_slist_free_all)*>;

    class libCURLHandleMixin {

        // 处理libcurl的全局初始化
        static std::atomic<bool> global_init_status;
        static std::atomic<bool> global_clean_status;
        static std::mutex global_execute;
        static std::lock_guard<std::mutex> global_execute_lock;

        inline static void libcurl_global_init() {
            if (libCURLHandleMixin::global_init_status) {
                return;
            }

            std::lock_guard<std::mutex> lock(libCURLHandleMixin::global_execute);

            if (libCURLHandleMixin::global_init_status) {
                return;
            }

            const auto res = curl_global_init(CURL_GLOBAL_ALL);

            if (res != CURLE_OK) {
                const auto err = curl::LIBCURL_CODE.at(res);
                throw std::runtime_error(err.c_str());
            }
        }

        inline static void libcurl_global_clean() {
            if (libCURLHandleMixin::global_clean_status) return;

            std::lock_guard<std::mutex> lock(libCURLHandleMixin::global_execute);

            if (libCURLHandleMixin::global_clean_status) return;

            curl_global_cleanup();
        }

    public:
        inline static void init() {
            libCURLHandleMixin::libcurl_global_init();
        }

        inline static void close() {
            libCURLHandleMixin::libcurl_global_clean();
        }

        // libcurl的API处理
        inline static CURLptrType init_curl_request() {
            CURL* curl_request = curl_easy_init();

            if (curl_request == nullptr) {
                spdlog::error("Init CURL failed...");
                throw std::runtime_error("Init CURL failed. ");
            }

            return CURLptrType(curl_request, curl_easy_cleanup);
        }

        template<typename T>
        inline static void curl_set_option(CURL* request, CURLoption key, T params) {
            const auto res = curl_easy_setopt(request, key, params);

            if (res != CURLE_OK) {
                const auto err = curl::LIBCURL_CODE.at(res);
                throw std::runtime_error(err.c_str());
            }
        }

        inline static CURLsListType build_header(const HeaderMap& header_map) {
            curl_slist* slist_buffer = nullptr;

            for (auto& item: header_map) {
                const auto content = item.first + ": " + item.second;
                slist_buffer = curl_slist_append(slist_buffer, content.c_str());
            }

            return CURLsListType(slist_buffer, curl_slist_free_all);
        }
    };

    class PreparedRequest : private libCURLHandleMixin {

    private:
        CURLptrType _curl_request_handler;

        Request _request;
        StreamBuffer _buffer;

        CURLsListType raw_header;
        std::string _final_url;

        template<typename T>
        inline void curlSetOption(CURLoption key, T params) {
            return libCURLHandleMixin::curl_set_option(this->_curl_request_handler.get(), key, params);
        }

    public:
        explicit PreparedRequest(Request request) :
                _curl_request_handler(PreparedRequest::init_curl_request()),
                _request(std::move(request)),
                _buffer(StreamBuffer(this->_request)),
                raw_header(build_header(this->_request.headers())),
                _final_url(this->_request.url().request_uri()) {

            //请求超时时长
            curlSetOption(CURLOPT_TIMEOUT_MS, this->_request.timeout());

            //连接超时时长
            curlSetOption(CURLOPT_CONNECTTIMEOUT, this->_request.timeout());

            //允许重定向
            curlSetOption(CURLOPT_FOLLOWLOCATION, static_cast<int>(this->_request.allow_redirects()));

            //关闭中断信号响应
            curlSetOption(CURLOPT_NOSIGNAL, static_cast<int>(true));

            //若启用，会将头文件的信息作为数据流输出
            curlSetOption(CURLOPT_HEADER, 1);

            //启用时会汇报所有的信息
            curlSetOption(CURLOPT_VERBOSE, static_cast<int>(this->request().verbose()));

            //需要获取的URL地址
            curlSetOption(CURLOPT_URL, this->_final_url.c_str());

            curlSetOption(CURLOPT_COOKIE, utils::params_encode<'=', ';'>(this->_request.cookies()));

            const auto& method = this->_request.method();

            if (method == curl::LIBCURL_HTTP_VERB.at(http::GET).c_str()) {

                curlSetOption(CURLOPT_HTTPGET, 1);

            } else if (method == curl::LIBCURL_HTTP_VERB.at(http::POST).c_str()) {

                curlSetOption(CURLOPT_HTTPPOST, 1);

                const auto& body = this->_request.body();

                curlSetOption(CURLOPT_POSTFIELDSIZE, body.size());

                curlSetOption(CURLOPT_POSTFIELDS, body.c_str());

            } else if (method == curl::LIBCURL_HTTP_VERB.at(http::HEAD).c_str()) {

                curlSetOption(CURLOPT_NOBODY, 1);

            }

            //得到请求结果后的回调函数
            curlSetOption(CURLOPT_HEADERFUNCTION, StreamBuffer::header_callback);

            curlSetOption(CURLOPT_WRITEFUNCTION, StreamBuffer::write_callback);

            curlSetOption(CURLOPT_HEADERDATA, &this->_buffer.header());

            curlSetOption(CURLOPT_WRITEDATA, &this->_buffer);
        }

        NOT_ALLOW_MODIFY_PROPERTY(request, _request, Request);

        NOT_ALLOW_MODIFY_PROPERTY(url, _final_url, std::string);

        EXPOSE_REF_GETTER(buffer, _buffer, StreamBuffer);

        EXPOSE_REF_GETTER(curl_request_handler, _curl_request_handler, CURLptrType);

        template<typename T>
        inline T curlGetInfo(CURLINFO key) {
            T buf;
            const auto res = curl_easy_getinfo(this->curl_request_handler().get(), key, &buf);

            if (res != CURLE_OK) {
                const auto err = curl::LIBCURL_CODE.at(res);
                throw std::runtime_error(err.c_str());
            }
            return buf;
        }
    };
}

#endif //CPPREQUESTS_PREPARED_REQUEST_H
