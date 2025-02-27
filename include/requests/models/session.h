#ifndef CPPREQUESTS_SESSION_H
#define CPPREQUESTS_SESSION_H

#include "requests/models/response.h"
#include "requests/models/request.h"
#include "requests/models/prepared_request.h"


namespace crq {

    class Session {

    public:
        Session() {};

        Response send(const Request& __req);

        inline void static perform(PreparedRequest& req) {
            //执行请求
            const auto res = curl_easy_perform(req.curl_request_handler().get());

            if (res != CURLE_OK) {
                const auto err = curl::LIBCURL_CODE.at(res);
                throw std::runtime_error(err.c_str());
            }
        }
    };

    Response Session::send(const Request& __req) {
        auto req = PreparedRequest(__req);
        Session::perform(req);

        const auto header = req.buffer().header();
        const auto content = req.buffer().body();

        const auto res_code = req.curlGetInfo<long>(CURLINFO_RESPONSE_CODE);
        const auto elapsed = req.curlGetInfo<double>(CURLINFO_TOTAL_TIME);

        return Response(std::move(req),
                        res_code,
                        header,
                        "utf-8",
                        content,
                        elapsed);
    }
}

#endif //CPPREQUESTS_SESSION_H
