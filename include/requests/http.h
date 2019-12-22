#ifndef CPPREQUESTS_HTTP_H
#define CPPREQUESTS_HTTP_H

#include <map>

namespace crq::http {

    typedef enum {
        OK = 200,
        BAD_REQUEST = 400,
        UNAUTHORIZED = 401,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        INTERNAL_SERVER_ERROR = 500,
        SERVER_UNAVAILABLE = 503,
    } HTTP_STATUS_CODE;

    static const std::map<int, std::string> HTTP_STATUS_CODE_MESSAGE = {
            {HTTP_STATUS_CODE::OK,                    "OK"},
            {HTTP_STATUS_CODE::BAD_REQUEST,           "Bad Request"},
            {HTTP_STATUS_CODE::UNAUTHORIZED,          "Unauthorized"},
            {HTTP_STATUS_CODE::FORBIDDEN,             "Forbidden"},
            {HTTP_STATUS_CODE::NOT_FOUND,             "Not Found"},
            {HTTP_STATUS_CODE::INTERNAL_SERVER_ERROR, "Internal Server Error"},
            {HTTP_STATUS_CODE::SERVER_UNAVAILABLE,    "Server Unavailable"}
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
