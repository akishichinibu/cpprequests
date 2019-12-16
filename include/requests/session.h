#ifndef CPPREQUESTS_SESSION_H
#define CPPREQUESTS_SESSION_H

#include <vector>
#include <requests/backend/libcurl.h>


namespace crq {

    class Session {

        std::vector<std::pair<std::string, std::string>> headers;
        bool stream = false;
        bool verify = false;
        int max_redirects = 10;
        bool trust_env = true;

        Session() = default;

        Response get(std::string &url,
                const std::map<std::string, std::string> &headers,
                int timeout = 3,
                bool allow_redirects = true) {

            CURLcode res = curl_global_init(CURL_GLOBAL_ALL);

            if (CURLE_OK != res) {
                std::cout << "curl init failed" << std::endl;
                return 1;
            }

            CURL *curl_request = curl_easy_init();

            if (curl_request == nullptr) {
                std::cout << "Init CURL failed..." << std::endl;
                return -1;
            }

            //请求超时时长
            curl_easy_setopt(curl_request, CURLOPT_TIMEOUT, timeout);

            //连接超时时长
            curl_easy_setopt(curl_request, CURLOPT_CONNECTTIMEOUT, timeout);

            //允许重定向
            curl_easy_setopt(curl_request, CURLOPT_FOLLOWLOCATION, static_cast<int>(allow_redirects));

            //若启用，会将头文件的信息作为数据流输出
            curl_easy_setopt(curl_request, CURLOPT_HEADER, 0L);

            //得到请求结果后的回调函数
            curl_easy_setopt(curl_request, CURLOPT_WRITEFUNCTION, write_callback);

            //请求结果的保存格式
            std::string buffer;
            curl_easy_setopt(curl_request, CURLOPT_WRITEDATA, &buffer);

            //关闭中断信号响应
            curl_easy_setopt(curl_request, CURLOPT_NOSIGNAL, 1L);

            //启用时会汇报所有的信息
            curl_easy_setopt(curl_request, CURLOPT_VERBOSE, 0L);

            curl_easy_setopt(curl_request, CURLOPT_URL, url.c_str()); //需要获取的URL地址

            curl_slist *pList = nullptr;

            for (auto &item: headers) {
                pList = curl_slist_append(pList, (item.first + ":" + item.second).c_str());
            }

            curl_easy_setopt(curl_request, CURLOPT_HTTPHEADER, pList);

            res = curl_easy_perform(curl_request);  //执行请求

            long res_code = 0;
            res = curl_easy_getinfo(curl_request, CURLINFO_RESPONSE_CODE, &res_code);

            //正确响应后，请请求转写成本地文件的文件
            if ((res == CURLE_OK) && (res_code == 200 || res_code == 201)) {
                std::cout << buffer << std::endl;
                return true;
            }

            curl_slist_free_all(pList);
            curl_easy_cleanup(curl_request);
            curl_global_cleanup();
            return 0;


        }




    };
}

#endif //CPPREQUESTS_SESSION_H
