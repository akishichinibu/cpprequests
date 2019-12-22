#ifndef CPPREQUESTS_URL_H
#define CPPREQUESTS_URL_H

#include <string>
#include <memory>

#include <curl/curl.h>
#include <stdexcept>

#include "requests/utils.h"
#include "requests/exception.h"

#define LIBCURL_URL_ERROR_CHECK(expr) _ERROR_CHECK(expr, crq::curl::LIBCURL_URL_CODE, CURLUE_OK, crq::InvalidURL)

namespace crq {

    class URL {

    private:
        using CURLUptrType = std::unique_ptr<CURLU, decltype(curl_url_cleanup) *>;
        using CURLCptrType = std::unique_ptr<char, decltype(curl_free) *>;
        using ParamsMap = std::map<std::string, std::string>;

        std::string _scheme;
        std::string _auth;

        std::string _host;
        unsigned short _port = 0;

        std::string _path;
        ParamsMap _params;
        std::string _fragment;

        inline static void _curl_url_set(CURLUptrType &handler,
                                         CURLUPart part,
                                         const char *content,
                                         unsigned int flags) {
            const auto res = curl_url_set(handler.get(), part, content, flags);

            if (res != CURLUE_OK) {
                std::string err, err_info;
                std::tie(err, err_info) = curl::LIBCURL_URL_CODE.at(res);
                throw std::runtime_error(err);
            }
        }

        inline static void _curl_url_set(CURLUptrType &handler,
                                         CURLUPart part,
                                         const std::string &content,
                                         unsigned int flags) {
            return _curl_url_set(handler, part, content.c_str(), flags);
        }

        inline static CURLCptrType _curl_url_get(CURLUptrType &handler,
                                                 CURLUPart part,
                                                 unsigned int flags) {
            char *buf;
            const auto res = curl_url_get(handler.get(), part, &buf, flags);

            if (res != CURLUE_OK) {
                curl_free(buf);
                return CURLCptrType(nullptr, curl_free);
            }

            return CURLCptrType(buf, curl_free);
        }

        inline static URL parse_from_string(const std::string &s) {
            auto h = curl_url();

            if (h == nullptr) {
                throw exception::InvalidURL("Exception when libcurl parse the url. ");
            }

            auto handler = CURLUptrType(h, curl_url_cleanup);

            URL::_curl_url_set(handler,
                               CURLUPART_URL,
                               s.c_str(),
                               CURLU_NON_SUPPORT_SCHEME);

            auto scheme = URL::_curl_url_get(handler, CURLUPART_SCHEME, 0);

            const std::string scheme_str = lower(std::string{scheme.get()});

            const auto host = URL::_curl_url_get(handler, CURLUPART_HOST, 0);

            const std::string host_str = lower(std::string{host.get()});

            auto get_or_empty = [](const CURLCptrType &ptr) -> std::string {
                std::string buf;
                if (ptr != nullptr) {
                    buf = ptr.get();
                }
                return buf;
            };

            const auto port = get_or_empty(URL::_curl_url_get(handler, CURLUPART_PORT, 0));

            const unsigned short port_num = port.empty() ? 0 : static_cast<unsigned short>(std::stol(port));

            const auto path = get_or_empty(URL::_curl_url_get(handler, CURLUPART_PATH, 0));

            const auto query = get_or_empty(URL::_curl_url_get(handler, CURLUPART_QUERY, 0));

            const auto fragment = get_or_empty(URL::_curl_url_get(handler, CURLUPART_FRAGMENT, 0));

            return {
                    scheme_str,
                    host_str,
                    port_num,
                    path,
                    query,
                    fragment
            };
        }


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

        inline std::string build_raw_query() const {
            std::string buf;

            if (!this->_params.empty()) {
                buf.reserve(10 * this->_params.size());

                for (auto& item: this->_params) {
                    const auto part = fmt::format("{0:s}={1:s}&", item.first, item.second);
                    buf += part;
                }

                buf.erase(buf.size() - 1, 1);
            }
            
            return buf;
        }

    public:
        URL(std::string scheme,
            std::string host,
            unsigned short port,
            std::string path = "",
            const std::string& query = "",
            std::string fragment = "") :
                _scheme(std::move(scheme)),
                _host(std::move(host)),
                _port(port),
                _path(std::move(path)),
                _params(parse_queries(query)),
                _fragment(std::move(fragment)) {}

        explicit inline URL(const std::string &url_str) {
            *this = std::move(URL::parse_from_string(url_str));
        }

        explicit operator std::string() {
            return this->request_uri();
        }

        inline std::string request_uri() const {
            std::string buf = fmt::format("{0:s}://{1:s}", this->_scheme, this->_host);
            buf.reserve(buf.size() * 2);

            if (this->_port > 0) {
                buf += fmt::format(":{0:d}", this->_port);
            }

            if (!this->_path.empty()) {
                buf += this->_path;
            }

            const auto query_str = this->raw_query();

            if (!query_str.empty()) {
                buf += fmt::format("?{0:s}", query_str);
            }

            if (!this->_fragment.empty()) {
                buf += fmt::format("#{0:s}", this->_fragment);
            }

            return buf;
        }

        CONST_REF_PROPERTY(scheme, _scheme, std::string);

        CONST_REF_PROPERTY(host, _host, std::string);

        CONST_REF_PROPERTY(port, _port, unsigned short);

        CONST_REF_PROPERTY(path, _path, std::string);

        CONST_REF_PROPERTY(fragment, _fragment, std::string);

        [[nodiscard]] inline std::string raw_query() const {
            return this->build_raw_query();
        }

        inline const ParamsMap& params() {
            return this->_params;
        }

        inline URL add_query(const std::string& key, const std::string& value) {
            this->_params[key] = value;
            return *this;
        }
    };
};

#endif //CPPREQUESTS_URL_H
