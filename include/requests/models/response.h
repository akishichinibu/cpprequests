#ifndef CPPREQUESTS_RESPONSE_H
#define CPPREQUESTS_RESPONSE_H

#include <string>

#include "requests/http.h"
#include "requests/utils.h"
#include "requests/models/prepared_request.h"


namespace crq {

    class Response {

    private:
        PreparedRequest _request;

        int _status_code;
        HeaderMap _headers;
        std::string _encoding;
        std::string _content;
        std::string _history;
        long _elapsed;

    public:
        Response(PreparedRequest request,
                 int status_code,
                 HeaderMap headers,
                 std::string encoding,
                 std::string content,
                 long elapsed);

        [[nodiscard]] inline std::string url() const {
            return this->_request.url();
        }

        PRIMARY_PROPERTY(status_code, _status_code, int);

        CONST_REF_PROPERTY(encoding, _encoding, std::string);

        CONST_REF_PROPERTY(content, _content, std::string);

        PRIMARY_PROPERTY(elapsed, _elapsed, double);

        CONST_REF_PROPERTY(headers, _headers, HeaderMap);

        [[nodiscard]] inline bool ok() const {
            return this->_status_code == http::OK;
        }

        [[nodiscard]] inline nlohmann::json json() const {
            return nlohmann::json::parse(this->_content);
        }

        [[nodiscard]] inline std::string text() const {
            return this->_content;
        }
    };

    Response::Response(PreparedRequest request,
                       int status_code,
                       HeaderMap headers,
                       std::string encoding,
                       std::string content,
                       long elapsed) :
            _request(std::move(request)),
            _status_code(status_code),
            _headers(std::move(headers)),
            _encoding(std::move(encoding)),
            _content(std::move(content)),
            _elapsed(elapsed) {}
}

#endif //CPPREQUESTS_RESPONSE_H
