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

    class PreparedRequest {

    private:
        using CURLptrType = std::unique_ptr<CURL, decltype(curl_easy_cleanup) *>;
        using CURLsListType = std::unique_ptr<curl_slist, decltype(curl_slist_free_all) *>;

        CURLptrType curl_request_handler;

        Request _request;
        StreamBuffer _buffer;

        CURLsListType raw_header;
        std::string _final_url;

        // 处理libcurl的全局初始化
        static std::atomic<bool> global_init_status;
        static std::atomic<bool> global_clean_status;
        static std::mutex global_execute;
        static std::lock_guard<std::mutex> global_execute_lock;

        // 统一的资源析构控制接口
        inline static void init();

        inline static void close();

        inline static void libcurl_global_init();

        inline static void libcurl_global_clean();

        // libcurl的API处理
        inline static CURLptrType init_curl_request();

        template<typename T>
        inline static void curl_set_option(CURL *request, CURLoption key, T params);

        inline static CURLsListType build_header(const HeaderMap &header_map);

    public:
        explicit PreparedRequest(Request request) :
                curl_request_handler(PreparedRequest::init_curl_request()),
                _request(std::move(request)),
                _buffer(StreamBuffer(this->_request)),
                raw_header(build_header(this->_request.headers())),
                _final_url(this->_request.url().request_uri()) {

            // 获取请求对象的指针
            auto curl_request = curl_request_handler.get();

            //请求超时时长
            curl_set_option(curl_request,
                            CURLOPT_TIMEOUT_MS,
                            this->_request.timeout());

            //连接超时时长
            curl_set_option(curl_request, CURLOPT_CONNECTTIMEOUT, this->_request.timeout());

            //允许重定向
            curl_set_option(curl_request,
                            CURLOPT_FOLLOWLOCATION,
                            static_cast<int>(this->_request.allow_redirects()));

            //关闭中断信号响应
            curl_easy_setopt(curl_request,
                             CURLOPT_NOSIGNAL,
                             static_cast<int>(true));

            const auto verbose = this->request().verbose();

            //若启用，会将头文件的信息作为数据流输出
            curl_set_option(curl_request, CURLOPT_HEADER, 1);

            //启用时会汇报所有的信息
            curl_set_option(curl_request, CURLOPT_VERBOSE, static_cast<int>(verbose));

            //需要获取的URL地址
            curl_set_option(curl_request, CURLOPT_URL, this->_final_url.c_str());

            const auto &method = this->_request.method();

            if (method == curl::LIBCURL_HTTP_VERB.at(http::GET)) {

                curl_set_option(curl_request, CURLOPT_HTTPGET, 1);

            } else if (method == curl::LIBCURL_HTTP_VERB.at(http::POST)) {

                curl_set_option(curl_request, CURLOPT_HTTPPOST, 1);

                const auto &body = this->_request.body();

                curl_set_option(curl_request, CURLOPT_POSTFIELDSIZE, body.size());

                curl_set_option(curl_request, CURLOPT_POSTFIELDS, body.c_str());

            } else if (method == curl::LIBCURL_HTTP_VERB.at(http::HEAD)) {

                curl_set_option(curl_request, CURLOPT_NOBODY, 1);

            }

            //得到请求结果后的回调函数
            curl_set_option(curl_request,
                            CURLOPT_HEADERFUNCTION,
                            StreamBuffer::header_callback);

            curl_set_option(curl_request,
                            CURLOPT_WRITEFUNCTION,
                            StreamBuffer::write_callback);

            curl_set_option(curl_request,
                            CURLOPT_HEADERDATA,
                            &this->_buffer.header());

            curl_set_option(curl_request,
                            CURLOPT_WRITEDATA,
                            &this->_buffer);
        }

        NOT_ALLOW_MODIFY_PROPERTY(request, _request, Request);

        NOT_ALLOW_MODIFY_PROPERTY(url, _final_url, std::string);

        EXPOSE_REF_GETTER(buffer, _buffer, StreamBuffer);

        inline CURL *curl_request_ptr() {
            return this->curl_request_handler.get();
        }

        template<typename T>
        inline T curl_get_info(CURLINFO key) {
            T buf;
            const auto res = curl_easy_getinfo(this->curl_request_ptr(), key, &buf);

            if (res != CURLE_OK) {
                const auto err = curl::LIBCURL_CODE.at(res);
                throw std::runtime_error(err.c_str());
            }
            return buf;
        }
    };


    void PreparedRequest::libcurl_global_init() {
        // curl_global_init 方法线程不安全

        if (PreparedRequest::global_init_status) {
            return;
        }

        std::lock_guard<std::mutex> lock(PreparedRequest::global_execute);

        if (PreparedRequest::global_init_status) {
            return;
        }

        const auto res = curl_global_init(CURL_GLOBAL_ALL);

        if (res != CURLE_OK) {
            const auto err = curl::LIBCURL_CODE.at(res);
            throw std::runtime_error(err.c_str());
        }
    }

    void PreparedRequest::libcurl_global_clean() {
        if (PreparedRequest::global_clean_status) {
            return;
        }

        std::lock_guard<std::mutex> lock(PreparedRequest::global_execute);

        if (PreparedRequest::global_clean_status) {
            return;
        }

        curl_global_cleanup();
    }

    auto PreparedRequest::init_curl_request() -> CURLptrType {
        CURL *curl_request = curl_easy_init();

        if (curl_request == nullptr) {
            spdlog::error("Init CURL failed...");
            throw std::runtime_error("Init CURL failed. ");
        }

        return CURLptrType(curl_request, curl_easy_cleanup);
    }

    void PreparedRequest::init() {
        PreparedRequest::libcurl_global_init();
    }

    void PreparedRequest::close() {
        PreparedRequest::libcurl_global_clean();
    }

    template<typename T>
    void PreparedRequest::curl_set_option(CURL *request, CURLoption key, T params) {
        const auto res = curl_easy_setopt(request, key, params);

        if (res != CURLE_OK) {
            const auto err = curl::LIBCURL_CODE.at(res);
            throw std::runtime_error(err.c_str());
        }
    }

    auto PreparedRequest::build_header(const HeaderMap &header_map) -> CURLsListType {
        curl_slist *slist_buffer = nullptr;

        for (auto &item: header_map) {
            const auto content = fmt::format("{0:s}: {1:s}", item.first, item.second);
            slist_buffer = curl_slist_append(slist_buffer, content.c_str());
        }

        return CURLsListType(slist_buffer, curl_slist_free_all);
    }
}

#endif //CPPREQUESTS_PREPARED_REQUEST_H
