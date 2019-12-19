#ifndef CPPREQUESTS_HTTP_H
#define CPPREQUESTS_HTTP_H

#include <map>

namespace crq::http {

    typedef enum {
        OK = 200,
        BAD_REQUEST = 400,
        UNAUTHORIZED = 401
    } HTTP_STATUS_CODE;

    static const std::map<int, std::string> HTTP_STATUS_CODE_MESSAGE = {
            {HTTP_STATUS_CODE::OK,           "OK"},
            {HTTP_STATUS_CODE::BAD_REQUEST,  "Bad Request"},
            {HTTP_STATUS_CODE::UNAUTHORIZED, "Unauthorized"},
            {403,                            "Forbidden"},
            {404,                            "Not Found"},
            {500,                            "Internal Server Error"},
            {503,                            "Server Unavailable"}
    };

    typedef enum {
        GET,
        HEAD,
        POST,
        PUT,
        DELETE_,
        CONNECT,
        OPTIONS,
        TRACE,
    } HTTP_REQUEST_TYPE;


};

#endif //CPPREQUESTS_HTTP_H
