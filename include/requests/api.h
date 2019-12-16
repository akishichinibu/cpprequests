#ifndef CPPREQUESTS_API_H
#define CPPREQUESTS_API_H

#include "requests/models/response.h"
#include "requests/backend/libcurl.h"

namespace crq {

    Response get(std::string &url,
                 int timeout = 3,
                 bool allow_redirects = true,
                 const HeaderMap &headers = default_headers()) {

        auto client = libcurlAdapter();
        auto ret = client.get(url, headers);
        const auto body = std::string(ret.stream_buffer.body());
        return Response(ret.url, ret.status_code, {}, "utf-8", body);
    }
}

#endif //CPPREQUESTS_API_H
