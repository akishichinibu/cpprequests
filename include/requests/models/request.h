#ifndef CPPREQUESTS_REQUEST_H
#define CPPREQUESTS_REQUEST_H

#include "requests/http.h"
#include "requests/curl.h"
#include "requests/utils.h"

#include <nlohmann/json.hpp>
#include <utility>
#include "requests/const.h"


namespace crq {

    class Request {

    private:
        http::HTTP_REQUEST_TYPE _method;
        std::string _url;

        int _timeout;
        bool _allow_redirects;
        bool _stream;

        bool _no_signal;
        bool _verbose;

        std::string _body;
        HeaderMap _headers;

    public:
        // Request的描述
        Request(http::HTTP_REQUEST_TYPE method,
                std::string url,
                const HeaderMap &headers = HeaderMap(),
                int timeout = 3,
                bool allow_redirects = true,
                bool stream = false,
                bool no_signal = false,
                bool verbose = false);

        Request(http::HTTP_REQUEST_TYPE method,
                std::string url,
                bool allow_redirects = true,
                bool stream = false,
                bool no_signal = false,
                bool verbose = false);

        PRIMARY_PROPERTY(method, _method, http::HTTP_REQUEST_TYPE);

        CONST_REF_PROPERTY(url, _url, std::string);

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

        inline Request &params_(const std::map<std::string, std::string> &p) {
            std::string buf;

            for (auto& item: p) {
                buf += fmt::format("{0:s}={1:s}&", item.first, item.second);
            }

            buf.erase(buf.size() - 1, 1);
            return this->body_(buf);
        }
    };

    Request::Request(http::HTTP_REQUEST_TYPE method,
                     std::string url,
                     const HeaderMap& headers,
                     int timeout,
                     bool allow_redirects,
                     bool stream,
                     bool no_signal,
                     bool verbose) :
            _method(method),
            _url(std::move(url)),
            _headers(merge_headers(default_headers(), headers)),
            _timeout(timeout),
            _allow_redirects(allow_redirects),
            _stream(stream),
            _no_signal(no_signal),
            _verbose(verbose) {};

    Request::Request(http::HTTP_REQUEST_TYPE method,
                     std::string url,
                     bool allow_redirects,
                     bool stream,
                     bool no_signal,
                     bool verbose):
                     Request(method,
                             std::move(url),
                             HeaderMap(),
                             3,
                             allow_redirects,
                             stream,
                             no_signal,
                             verbose) {}
};

#endif //CPPREQUESTS_REQUEST_H
