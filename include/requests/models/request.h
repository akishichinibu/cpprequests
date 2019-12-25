#ifndef CPPREQUESTS_REQUEST_H
#define CPPREQUESTS_REQUEST_H

#include <nlohmann/json.hpp>
#include <utility>

#include "requests/utils.h"
#include "requests/const.h"
#include "requests/models/url.h"


namespace crq {

    class Request {
        /// User used Request object.

    public:
        /// The definition of default params for request
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

        std::string _body;
        HeaderMap _headers;
        std::map<std::string, std::string> _proxy;

    public:
        /// The constructor of the Request.
        /// @param method - The request method as string. The self-defined request types are also allowed.
        /// @param url - The target url to request.
        /// @param headers - The user-defined headers to append to request.
        /// @param proxy - The proxy configuration.
        /// @param timeout - The timeout setting. Default is 300ms.
        /// @param allow_redirects - The timeout setting. Default is 300ms.
        /// @param stream - The timeout setting. Default is 300ms.
        /// @param no_signal - The timeout setting. Default is 300ms.
        /// @param verbose - The timeout setting. Default is 300ms.
        Request(const std::string &method, const std::string &url) :
                _method(string::upper(method)),
                _url(URL{url}),
                _headers(utils::default_headers()),
                _timeout(DEFAULT_TIMEOUT),
                _allow_redirects(DEFAULT_ALLOW_REDIRECTS),
                _stream(DEFAULT_VERBOSE),
                _verbose(DEFAULT_VERBOSE) {};

        ALLOW_MODIFY_PROPERTY(timeout, _timeout, long);

        ALLOW_MODIFY_PROPERTY(allow_redirects, _allow_redirects, bool);

        ALLOW_MODIFY_PROPERTY(stream, _stream, bool);

        ALLOW_MODIFY_PROPERTY(verbose, _verbose, bool);

        ALLOW_MODIFY_PROPERTY(headers, _headers, HeaderMap);

        ALLOW_MODIFY_PROPERTY(body, _body, std::string);

        NOT_ALLOW_MODIFY_PROPERTY(method, _method, std::string);

        NOT_ALLOW_MODIFY_PROPERTY(url, _url, URL);

        inline Request &json(const nlohmann::json &buf) {
            const auto json_string = buf.dump();
            return this->body(json_string);
        }

        inline Request &params(const ParamsMap &p) {
            this->_url.params(p);
            return *this;
        }
    };
};

#endif //CPPREQUESTS_REQUEST_H
