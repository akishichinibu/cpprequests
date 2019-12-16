#include "requests/backend/libcurl.h"
#include <spdlog/spdlog.h>
#include <iostream>
#include "requests/utils.h"


namespace crq {

    libcurlAdapter::libcurlAdapter(int timeout,
                                   bool allow_redirects,
                                   bool stream) :
            timeout(timeout),
            allow_redirects(allow_redirects),
            stream(stream) {}

    void libcurlAdapter::libcurl_global_init() {
        if (libcurlAdapter::global_init_status) {
            return;
        }

        std::lock_guard<std::mutex> lock(libcurlAdapter::global_execute);

        if (libcurlAdapter::global_init_status) {
            return;
        }

        LIBCURL_ERROR_CHECK(curl_global_init(CURL_GLOBAL_ALL));
    }

    void libcurlAdapter::libcurl_global_clean() {
        std::lock_guard<std::mutex> lock(libcurlAdapter::global_execute);

        if (libcurlAdapter::global_clean_status) {
            return;
        }

        curl_global_cleanup();
    }

    CURLptrType libcurlAdapter::init_request() {
        CURL *curl_request = curl_easy_init();

        if (curl_request == nullptr) {
            spdlog::error("Init CURL failed...");
            throw std::runtime_error("Init CURL failed. ");
        }

        return CURLptrType(curl_request, curl_easy_cleanup);
    }


    std::size_t libcurlAdapter::header_callback(void *contents,
                                                std::size_t size,
                                                std::size_t nmemb,
                                                void *userdata) {

        const std::size_t content_length = size * nmemb;

        if (content_length > 22) {
            auto convert_userdata = (HeaderMap *) (userdata);

            std::string buf((char *) contents);

            const auto d = buf.find(':');

            const auto key = buf.substr(0, d);
            auto value = buf.substr(d + 1, buf.size() - 1);

            if (value[0] == ' ') {
                value.erase(0, 1);
            }

            convert_userdata->operator[](key) = value;
        }

        return content_length;
    }


    void libcurlAdapter::init() {
        libcurlAdapter::libcurl_global_init();
    }

    void libcurlAdapter::close() {
        libcurlAdapter::libcurl_global_clean();
    }

    void curl_request_config(CURL *curl_request,
                             int timeout,
                             bool allow_redirects,
                             int no_signal,
                             int verbose) {

        //请求超时时长
        LIBCURL_ERROR_CHECK(curl_easy_setopt(curl_request, CURLOPT_TIMEOUT, timeout));

        //连接超时时长
        LIBCURL_ERROR_CHECK(curl_easy_setopt(curl_request, CURLOPT_CONNECTTIMEOUT, timeout));

        //允许重定向
        LIBCURL_ERROR_CHECK(curl_easy_setopt(curl_request,
                CURLOPT_FOLLOWLOCATION,
                static_cast<int>(allow_redirects)));

        //若启用，会将头文件的信息作为数据流输出
        LIBCURL_ERROR_CHECK(curl_easy_setopt(curl_request, CURLOPT_HEADER, 1L));

        //关闭中断信号响应
        LIBCURL_ERROR_CHECK(curl_easy_setopt(curl_request,
                CURLOPT_NOSIGNAL,
                static_cast<int>(no_signal)));

        //启用时会汇报所有的信息
        LIBCURL_ERROR_CHECK(curl_easy_setopt(curl_request,
                CURLOPT_VERBOSE,
                static_cast<int>(verbose)));
    }

    template<typename T>
    inline T curl_get_info(CURL* request, CURLINFO key) {
        T buf;
        LIBCURL_ERROR_CHECK(curl_easy_getinfo(request, key, &buf));
        return buf;
    }

    curl_slist* HeaderMap2sList(const HeaderMap &headers) {
        curl_slist *sList = nullptr;

        for (auto &item: headers) {
            const auto content = fmt::format("{0}:{1}", item.first, item.second);
            sList = curl_slist_append(sList, content.c_str());
        }

        return sList;
    }


    RequestReturn libcurlAdapter::get(std::string &url, const HeaderMap &headers) {
        libcurlAdapter::libcurl_global_init();

        // 生成流式传输的handler
        auto stream_buffer = libcurlStreamBuffer(std::move(libcurlAdapter::init_request()));

        // 获取请求对象的指针
        auto curl_request = stream_buffer.request().get();

        curl_request_config(curl_request,
                            this->timeout,
                            this->allow_redirects,
                            false,
                            true);

        //需要获取的URL地址
        LIBCURL_ERROR_CHECK(curl_easy_setopt(curl_request, CURLOPT_URL, url.c_str()));

        LIBCURL_ERROR_CHECK(curl_easy_setopt(curl_request, CURLOPT_CUSTOMREQUEST, "GET"));

        //得到请求结果后的回调函数
        LIBCURL_ERROR_CHECK(curl_easy_setopt(curl_request, CURLOPT_WRITEFUNCTION, libcurlStreamBuffer::write_callback));

        LIBCURL_ERROR_CHECK(curl_easy_setopt(curl_request, CURLOPT_HEADERFUNCTION, libcurlAdapter::header_callback));

        //请求结果的保存格式
        LIBCURL_ERROR_CHECK(curl_easy_setopt(curl_request, CURLOPT_HEADERDATA, &stream_buffer.header()));

        LIBCURL_ERROR_CHECK(curl_easy_setopt(curl_request, CURLOPT_WRITEDATA, &stream_buffer.body()));

        // 组装Header
        curl_slist *pList = HeaderMap2sList(headers);
        curl_easy_setopt(curl_request, CURLOPT_HTTPHEADER, pList);

        //执行请求
        LIBCURL_ERROR_CHECK(curl_easy_perform(curl_request));

        // 释放Header数据区
        curl_slist_free_all(pList);

        // 获取状态码
        long res_code = 0;
        LIBCURL_ERROR_CHECK(curl_easy_getinfo(curl_request, CURLINFO_RESPONSE_CODE, &res_code));

        return {url, res_code, std::move(stream_buffer)};
    }

    RequestReturn libcurlAdapter::post(std::string &url,
            const std::string& params,
            const HeaderMap &headers) {
        libcurlAdapter::libcurl_global_init();

        auto stream_buffer = libcurlStreamBuffer(std::move(libcurlAdapter::init_request()));
        auto curl_request = stream_buffer.request().get();

        curl_request_config(curl_request,
                            this->timeout,
                            this->allow_redirects,
                            false,
                            true);

        //需要获取的URL地址
        curl_easy_setopt(curl_request, CURLOPT_URL, url.c_str());

        curl_easy_setopt(curl_request, CURLOPT_CUSTOMREQUEST, "POST");

        // 处理请求体
        curl_easy_setopt(curl_request, CURLOPT_POSTFIELDS, params.c_str());

        //得到请求结果后的回调函数
        curl_easy_setopt(curl_request, CURLOPT_WRITEFUNCTION, libcurlStreamBuffer::write_callback);

        curl_easy_setopt(curl_request, CURLOPT_HEADERFUNCTION, libcurlAdapter::header_callback);

        //请求结果的保存格式
        curl_easy_setopt(curl_request, CURLOPT_HEADERDATA, &stream_buffer.header());

        curl_easy_setopt(curl_request, CURLOPT_WRITEDATA, &stream_buffer.body());

        // 组装Header
        curl_slist *pList = HeaderMap2sList(headers);
        curl_easy_setopt(curl_request, CURLOPT_HTTPHEADER, pList);

        //执行请求
        curl_easy_perform(curl_request);

        curl_slist_free_all(pList);

        long res_code = curl_get_info<long>(curl_request, CURLINFO_RESPONSE_CODE);
        long total_time = curl_get_info<long>(curl_request, CURLINFO_TOTAL_TIME);

        return {
            url,
            res_code,
            std::move(stream_buffer),
            total_time
        };
    }

    RequestReturn libcurlAdapter::post(std::string &url,
            const nlohmann::json& params,
            const HeaderMap &headers) {
        return this->post(url, params.dump(), headers);
    }

    std::atomic<bool> libcurlAdapter::global_init_status = false;
    std::atomic<bool> libcurlAdapter::global_clean_status = false;
    std::mutex libcurlAdapter::global_execute = std::mutex();
}
