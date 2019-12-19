//#ifndef CPPREQUESTS_API_H
//#define CPPREQUESTS_API_H
//
//#include "requests/models/response.h"
//#include "requests/backend/libcurl.h"
//
//namespace crq {
//
//    Response get(const std::string &url,
//                 int timeout = 3,
//                 bool allow_redirects = true,
//                 bool stream = false,
//                 bool verbose = false,
//                 const HeaderMap &headers = default_headers()) {
//
//        auto client = libcurlAdapter(timeout, allow_redirects, stream);
//        auto ret = client.get(url, headers, verbose);
//
//        const auto body = std::string(ret.stream_buffer.body());
//
//        return Response(ret.url,
//                        ret.status_code,
//                        ret.stream_buffer.header(),
//                        "utf-8",
//                        body,
//                        ret.elapsed);
//    }
//}
//
//#endif //CPPREQUESTS_API_H
