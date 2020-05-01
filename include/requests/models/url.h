#ifndef CPPREQUESTS_URL_H
#define CPPREQUESTS_URL_H

#include <string>
#include <memory>

#include <curl/curl.h>
#include <stdexcept>

#include "requests/structures.h"
#include "requests/utils.h"
#include "requests/extends/_string.h"


namespace crq {

    class libCURLUrlHandleMixin {
    public:

        inline static void _curl_url_set(CURLUptrType& handler,
                                         CURLUPart part,
                                         const char* content,
                                         unsigned int flags) {
            const auto res = curl_url_set(handler.get(), part, content, flags);

            if (res != CURLUE_OK) {
                const auto err = curl::LIBCURL_URL_CODE.at(res);
                throw std::runtime_error(err.c_str());
            }
        }

        inline static void _curl_url_set(CURLUptrType& handler,
                                         CURLUPart part,
                                         const std::string& content,
                                         unsigned int flags) {
            return _curl_url_set(handler, part, content.c_str(), flags);
        }

        inline static CURLCptrType _curl_url_get(CURLUptrType& handler,
                                                 CURLUPart part,
                                                 unsigned int flags) {
            char* buf;
            const auto res = curl_url_get(handler.get(), part, &buf, flags);

            if (res != CURLUE_OK) {
                curl_free(buf);
                return CURLCptrType(nullptr, curl_free);
            }

            return CURLCptrType(buf, curl_free);
        }
    };

    class URL : libCURLUrlHandleMixin {

    private:
        std::string _scheme;
        std::string _auth;

        std::string _host;
        unsigned short _port = 0;

        std::string _path;
        ParamsMap _params;
        std::string _fragment;

        inline static ParamsMap parse_queries(const std::string& qstr) {
            if (qstr.empty()) {
                return {};
            }

            auto i = qstr.cbegin();
            auto j = qstr.cbegin();
            const auto end = qstr.cend();

            auto kvs = ParamsMap();

            while (true) {
                while (*i != '&') {
                    i++;
                    if (i == end) {
                        break;
                    }
                }

                auto k = j;

                while (*k != '=') {
                    k++;
                }

                const auto key = std::string(j, k);
                const auto value = std::string(k + 1, i);
                kvs[key] = value;

                if (i == end) {
                    break;
                }

                i = i + 1;
                j = i;
            }

            return kvs;
        }

    public:
        URL(std::string scheme,
            std::string host,
            unsigned short port,
            std::string path,
            std::string fragment = "") :
                _scheme(std::move(scheme)),
                _host(std::move(host)),
                _port(port),
                _path(std::move(path)),
                _fragment(std::move(fragment)) {};

        explicit inline URL(const std::string& url_str) {
            auto h = curl_url();

            if (h == nullptr) {
                throw std::invalid_argument("Exception when libcurl parse the url. ");
            }

            auto handler = CURLUptrType(h, curl_url_cleanup);

            URL::_curl_url_set(handler, CURLUPART_URL, url_str.c_str(), CURLU_NON_SUPPORT_SCHEME);

            auto scheme = std::string{URL::_curl_url_get(handler, CURLUPART_SCHEME, 0).get()} | string::lower();

            auto host = std::string{URL::_curl_url_get(handler, CURLUPART_HOST, 0).get()} | string::lower();

            auto get_or_empty = [](const CURLCptrType& ptr) -> std::string {
                if (ptr != nullptr) {
                    return std::string(ptr.get());
                } else {
                    return std::string();
                }
            };

            const auto port = get_or_empty(URL::_curl_url_get(handler, CURLUPART_PORT, 0));

            const unsigned short port_num = port.empty() ? 0 : static_cast<unsigned short>(std::stol(port));

            const auto path = get_or_empty(URL::_curl_url_get(handler, CURLUPART_PATH, 0));

            const auto fragment = get_or_empty(URL::_curl_url_get(handler, CURLUPART_FRAGMENT, 0));

            const auto query_str = get_or_empty(URL::_curl_url_get(handler, CURLUPART_QUERY, 0));

            this->_scheme = scheme;
            this->_host = host;
            this->_port = port_num;
            this->_path = path;
            this->_fragment = fragment;

            this->params(parse_queries(query_str));
        }

        explicit operator std::string() {
            return this->request_uri();
        }

        [[nodiscard]] inline std::string request_uri() const {
            std::string buf = this->_scheme + "://" + this->_host;
            buf.reserve(buf.size() * 2);

            if (this->_port > 0) {
                buf += fmt::format(":{0:d}", this->_port);
            }

            if (!this->_path.empty()) {
                buf += this->_path;
            }

            const auto query_str = this->query_string();

            if (!query_str.empty()) {
                buf += "?" + query_str;
            }

            if (!this->_fragment.empty()) {
                buf += "#" + this->_fragment;
            }

            return buf;
        }

        NOT_ALLOW_MODIFY_PROPERTY(scheme, _scheme, std::string);

        NOT_ALLOW_MODIFY_PROPERTY(host, _host, std::string);

        NOT_ALLOW_MODIFY_PROPERTY(port, _port, unsigned short);

        NOT_ALLOW_MODIFY_PROPERTY(path, _path, std::string);

        NOT_ALLOW_MODIFY_PROPERTY(fragment, _fragment, std::string);

        ALLOW_MODIFY_PROPERTY(params, _params, ParamsMap);

        [[nodiscard]] inline std::string query_string() const {
            std::string buf;

            if (!this->_params.empty()) {
                buf.reserve(10 * this->_params.size());

                for (auto& item: this->_params) {
                    const auto part = item.first + "=" + item.second + "&";
                    buf += part;
                }

                buf.erase(buf.size() - 1, 1);
            }

            return buf;
        }
    };
};

#endif //CPPREQUESTS_URL_H
