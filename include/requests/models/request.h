#ifndef CPPREQUESTS_REQUEST_H
#define CPPREQUESTS_REQUEST_H

#include <nlohmann/json.hpp>
#include <utility>

#include "requests/utils.h"
#include "requests/const.h"
#include "requests/models/url.h"
#include "requests/extends/_string.h"


namespace crq {

    class Request {

    public:
        static constexpr long DEFAULT_TIMEOUT = 300L;
        static constexpr bool DEFAULT_ALLOW_REDIRECTS = true;
        static constexpr bool DEFAULT_STREAM = false;
        static constexpr bool DEFAULT_VERBOSE = false;

    private:
        std::string _method;
        URL _url;

        long _timeout;
        bool _allow_redirects;
        bool _stream;

        bool _verbose;

        HeaderMap _headers;
        std::string _body;
        std::string _auth;
        ProxyMap _proxy;
        CookieMap _cookies;

        std::string

    public:
        Request(const std::string& method, const std::string& url) :
                _url(URL{url}),
                _headers(utils::default_headers()),
                _timeout(DEFAULT_TIMEOUT),
                _allow_redirects(DEFAULT_ALLOW_REDIRECTS),
                _stream(DEFAULT_VERBOSE),
                _verbose(DEFAULT_VERBOSE) {

            this->_method = method | string::upper();
        };

        ALLOW_MODIFY_PROPERTY(timeout, _timeout, long);

        ALLOW_MODIFY_PROPERTY(allow_redirects, _allow_redirects, bool);

        ALLOW_MODIFY_PROPERTY(stream, _stream, bool);

        ALLOW_MODIFY_PROPERTY(verbose, _verbose, bool);

        ALLOW_MODIFY_PROPERTY(headers, _headers, HeaderMap);

        ALLOW_MODIFY_PROPERTY(body, _body, std::string);

        ALLOW_MODIFY_PROPERTY(cookies, _cookies, ParamsMap);

        NOT_ALLOW_MODIFY_PROPERTY(method, _method, std::string);

        NOT_ALLOW_MODIFY_PROPERTY(url, _url, URL);

        inline Request& json(const nlohmann::json& buf) {
            return this->body(buf.dump());
        }

        inline Request& params(const ParamsMap& p) {
            this->_url.params(p);
            return *this;
        }

        inline Request& data(const ParamsMap& form) {
            return this->body(utils::params_encode<'=', '&'>(form));
        }
    };
}

#endif //CPPREQUESTS_REQUEST_H
