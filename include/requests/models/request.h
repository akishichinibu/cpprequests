#ifndef CPPREQUESTS_REQUEST_H
#define CPPREQUESTS_REQUEST_H

#include <nlohmann/json.hpp>
#include <utility>

#include "requests/http.h"
#include "requests/utils.h"
#include "requests/const.h"
#include "requests/models/url.h"


namespace crq {

    class Request {

    public:
        static constexpr int DEFAULT_TIMEOUT = 10;
        static constexpr bool DEFAULT_ALLOW_REDIRECTS = true;
        static constexpr bool DEFAULT_STREAM = false;
        static constexpr bool DEFAULT_VERBOSE = false;

    private:
        std::string _method;
        URL _url;

        int _timeout;
        bool _allow_redirects;
        bool _stream;

        bool _no_signal;
        bool _verbose;

        std::string _body;
        HeaderMap _headers;

    public:
        // Request的描述
        Request(const std::string& method,
                const std::string& url,
                const HeaderMap &headers = HeaderMap(),
                const std::map<std::string, std::string>& proxy = {},
                int timeout = DEFAULT_TIMEOUT,
                bool allow_redirects = DEFAULT_ALLOW_REDIRECTS,
                bool stream = DEFAULT_STREAM,
                bool no_signal = false,
                bool verbose = DEFAULT_VERBOSE);

        Request(const std::string& method,
                URL url,
                const HeaderMap &headers = HeaderMap(),
                const std::map<std::string, std::string>& proxy = {},
                int timeout = DEFAULT_TIMEOUT,
                bool allow_redirects = DEFAULT_ALLOW_REDIRECTS,
                bool stream = DEFAULT_STREAM,
                bool no_signal = false,
                bool verbose = DEFAULT_VERBOSE);

        Request(http::HTTP_REQUEST_TYPE method,
                const std::string& url,
                const HeaderMap &headers = HeaderMap(),
                const std::map<std::string, std::string>& proxy = {},
                int timeout = DEFAULT_TIMEOUT,
                bool allow_redirects = DEFAULT_ALLOW_REDIRECTS,
                bool stream = DEFAULT_STREAM,
                bool no_signal = false,
                bool verbose = DEFAULT_VERBOSE);

        Request(http::HTTP_REQUEST_TYPE method,
                URL url,
                const HeaderMap &headers = HeaderMap(),
                const std::map<std::string, std::string>& proxy = {},
                int timeout = DEFAULT_TIMEOUT,
                bool allow_redirects = DEFAULT_ALLOW_REDIRECTS,
                bool stream = DEFAULT_STREAM,
                bool no_signal = false,
                bool verbose = DEFAULT_VERBOSE);

        PRIMARY_PROPERTY(method, _method, std::string);

        CONST_REF_PROPERTY(url, _url, URL);

        PRIMARY_PROPERTY(timeout, _timeout, long);

        PRIMARY_PROPERTY(allow_redirects, _allow_redirects, bool);

        PRIMARY_PROPERTY(stream, _stream, bool);

        PRIMARY_PROPERTY(no_signal, _no_signal, bool);

        PRIMARY_PROPERTY(verbose, _verbose, bool);

        CONST_REF_PROPERTY(headers, _headers, HeaderMap);

        REF_PROPERTY(body, _body, std::string);

        inline Request &body_(const std::string &buf) {
            this->_body.append(buf);
            return *this;
        }

        inline Request &json_(const nlohmann::json &buf) {
            const auto json_string = buf.dump();
            return this->body_(json_string);
        }

        inline Request &params_(const std::vector<std::pair<std::string, std::string>> &p) {
            for (auto& item: p) {
                this->_url.add_query(item.first, item.second);
            }
            return *this;
        }
    };

    Request::Request(const std::string& method,
                     const std::string& url,
                     const HeaderMap& headers,
                     const std::map<std::string, std::string>& proxy,
                     int timeout,
                     bool allow_redirects,
                     bool stream,
                     bool no_signal,
                     bool verbose) :
            _method(string::upper(method)),
            _url(URL { url }),
            _headers(merge_headers(default_headers(), headers)),
            _timeout(timeout),
            _allow_redirects(allow_redirects),
            _stream(stream),
            _no_signal(no_signal),
            _verbose(verbose) {};

    Request::Request(const std::string& method,
                     URL url,
                     const HeaderMap& headers,
                     const std::map<std::string, std::string>& proxy,
                     int timeout,
                     bool allow_redirects,
                     bool stream,
                     bool no_signal,
                     bool verbose) :
            _method(string::upper(method)),
            _url(std::move(url)),
            _headers(merge_headers(default_headers(), headers)),
            _timeout(timeout),
            _allow_redirects(allow_redirects),
            _stream(stream),
            _no_signal(no_signal),
            _verbose(verbose) {};

    Request::Request(http::HTTP_REQUEST_TYPE method,
                     const std::string& url,
                     const HeaderMap& headers,
                     const std::map<std::string, std::string>& proxy,
                     int timeout,
                     bool allow_redirects,
                     bool stream,
                     bool no_signal,
                     bool verbose) :
            Request(curl::LIBCURL_HTTP_VERB.at(method),
                    url,
                    headers,
                    proxy,
                    timeout,
                    allow_redirects,
                    stream,
                    no_signal,
                    verbose) {};

    Request::Request(http::HTTP_REQUEST_TYPE method,
                     URL url,
                     const HeaderMap& headers,
                     const std::map<std::string, std::string>& proxy,
                     int timeout,
                     bool allow_redirects,
                     bool stream,
                     bool no_signal,
                     bool verbose) :
                     Request(curl::LIBCURL_HTTP_VERB.at(method),
                             std::move(url),
                             headers,
                             proxy,
                             timeout,
                             allow_redirects,
                             stream,
                             no_signal,
                             verbose) {};
};

#endif //CPPREQUESTS_REQUEST_H
