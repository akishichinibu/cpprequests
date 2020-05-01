#ifndef CPPREQUESTS_API_H
#define CPPREQUESTS_API_H

#include "requests/models/session.h"
#include "requests/models/response.h"

namespace crq {

    Response get(const std::string& url,
                 long timeout = 300,
                 const HeaderMap& headers = utils::default_headers(),
                 bool allow_redirects = true) {

        auto req = crq::Request("GET", url)
                .timeout(timeout)
                .headers(headers)
                .allow_redirects(allow_redirects);

        auto session = crq::Session();
        auto res = session.send(req);

        return res;
    }
}

#endif //CPPREQUESTS_API_H
