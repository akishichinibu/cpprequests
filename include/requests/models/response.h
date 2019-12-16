#ifndef CPPREQUESTS_RESPONSE_H
#define CPPREQUESTS_RESPONSE_H

#include <string>
#include "requests/utils.h"


namespace crq {

    class Response {

    private:
        std::string _url;
        int _status_code;
        HeaderMap _headers;
        std::string _encoding;
        std::string _content;
        std::string _history;
        long _elapsed;

    public:
        Response(const std::string& url,
                 int status_code,
                 const HeaderMap& headers,
                 const std::string& encoding,
                 const std::string& content,
                 long elapsed);

        [[nodiscard]] inline const std::string &url() const {
            return this->_url;
        }

        [[nodiscard]] inline const int &status_code() const {
            return this->_status_code;
        }

        [[nodiscard]] inline const std::string &encoding() const {
            return this->_encoding;
        }

        [[nodiscard]] inline bool ok() const {
            return this->_status_code == 200;
        }
    };
}

#endif //CPPREQUESTS_RESPONSE_H
