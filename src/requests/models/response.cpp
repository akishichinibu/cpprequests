#include "requests/models/response.h"

namespace crq {

    Response::Response(const std::string& url,
                       int status_code,
                       const HeaderMap& headers,
                       const std::string& encoding,
                       const std::string& content,
                       long elapsed) :
            _url(url),
            _status_code(status_code),
            _headers(headers),
            _encoding(encoding),
            _content(content),
            _elapsed(elapsed) {
    }
}