#ifndef CPPREQUESTS_CONST_H
#define CPPREQUESTS_CONST_H

#include <curl/curl.h>
#include <mapbox/eternal.hpp>

#include "requests/structures.h"

namespace crq {

    namespace http {

        /// @enum http status code definition
        typedef enum {
            OK = 200, ///< OK
            CREATED = 201, ///< OK
            BAD_REQUEST = 400,
            UNAUTHORIZED = 401,
            FORBIDDEN = 403,
            NOT_FOUND = 404,
            INTERNAL_SERVER_ERROR = 500,
            SERVER_UNAVAILABLE = 503,
        } HTTP_STATUS_CODE;


        /// @enum The message of the http status code definition
        constexpr const auto HTTP_STATUS_CODE_MESSAGE =
                mapbox::eternal::map<HTTP_STATUS_CODE, mapbox::eternal::string>({
                                                                                        {HTTP_STATUS_CODE::OK,                    "OK"},
                                                                                        {HTTP_STATUS_CODE::CREATED,               "CREATED"},
                                                                                        {HTTP_STATUS_CODE::BAD_REQUEST,           "Bad Request"},
                                                                                        {HTTP_STATUS_CODE::UNAUTHORIZED,          "Unauthorized"},
                                                                                        {HTTP_STATUS_CODE::FORBIDDEN,             "Forbidden"},
                                                                                        {HTTP_STATUS_CODE::NOT_FOUND,             "Not Found"},
                                                                                        {HTTP_STATUS_CODE::INTERNAL_SERVER_ERROR, "Internal Server Error"},
                                                                                        {HTTP_STATUS_CODE::SERVER_UNAVAILABLE,    "Server Unavailable"}
                                                                                });

        /// @enum http requests action definition
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


    namespace curl {

        constexpr const auto LIBCURL_CODE = mapbox::eternal::map<CURLcode, mapbox::eternal::string>({
                                                                                                            {CURLE_OK,                       "CURLE_OK"},
                                                                                                            {CURLE_UNSUPPORTED_PROTOCOL,     "CURLE_UNSUPPORTED_PROTOCOL"},
                                                                                                            {CURLE_FAILED_INIT,              "CURLE_FAILED_INIT"},
                                                                                                            {CURLE_URL_MALFORMAT,            "CURLE_URL_MALFORMAT"},
                                                                                                            {CURLE_NOT_BUILT_IN,             "CURLE_NOT_BUILT_IN"},
                                                                                                            {CURLE_COULDNT_RESOLVE_PROXY,    "CURLE_COULDNT_RESOLVE_PROXY"},
                                                                                                            {CURLE_COULDNT_RESOLVE_HOST,     "CURLE_COULDNT_RESOLVE_HOST"},
                                                                                                            {CURLE_COULDNT_CONNECT,          "CURLE_COULDNT_CONNECT"},
                                                                                                            {CURLE_WEIRD_SERVER_REPLY,       "CURLE_WEIRD_SERVER_REPLY"},
                                                                                                            {CURLE_REMOTE_ACCESS_DENIED,     "CURLE_REMOTE_ACCESS_DENIED"},
                                                                                                            {CURLE_FTP_ACCEPT_FAILED,        "CURLE_FTP_ACCEPT_FAILED"},
                                                                                                            {CURLE_FTP_WEIRD_PASS_REPLY,     "CURLE_FTP_WEIRD_PASS_REPLY"},
                                                                                                            {CURLE_FTP_ACCEPT_TIMEOUT,       "CURLE_FTP_ACCEPT_TIMEOUT"},
                                                                                                            {CURLE_FTP_WEIRD_PASV_REPLY,     "CURLE_FTP_WEIRD_PASV_REPLY"},
                                                                                                            {CURLE_FTP_WEIRD_227_FORMAT,     "CURLE_FTP_WEIRD_227_FORMAT"},
                                                                                                            {CURLE_FTP_CANT_GET_HOST,        "CURLE_FTP_CANT_GET_HOST"},
                                                                                                            {CURLE_HTTP2,                    "CURLE_HTTP2"},
                                                                                                            {CURLE_FTP_COULDNT_SET_TYPE,     "CURLE_FTP_COULDNT_SET_TYPE"},
                                                                                                            {CURLE_PARTIAL_FILE,             "CURLE_PARTIAL_FILE"},
                                                                                                            {CURLE_FTP_COULDNT_RETR_FILE,    "CURLE_FTP_COULDNT_RETR_FILE"},
                                                                                                            {CURLE_OBSOLETE20,               "CURLE_OBSOLETE20"},
                                                                                                            {CURLE_QUOTE_ERROR,              "CURLE_QUOTE_ERROR"},
                                                                                                            {CURLE_HTTP_RETURNED_ERROR,      "CURLE_HTTP_RETURNED_ERROR"},
                                                                                                            {CURLE_WRITE_ERROR,              "CURLE_WRITE_ERROR"},
                                                                                                            {CURLE_OBSOLETE24,               "CURLE_OBSOLETE24"},
                                                                                                            {CURLE_UPLOAD_FAILED,            "CURLE_UPLOAD_FAILED"},
                                                                                                            {CURLE_READ_ERROR,               "CURLE_READ_ERROR"},
                                                                                                            {CURLE_OUT_OF_MEMORY,            "CURLE_OUT_OF_MEMORY"},
                                                                                                            {CURLE_OPERATION_TIMEDOUT,       "CURLE_OPERATION_TIMEDOUT"},
                                                                                                            {CURLE_OBSOLETE29,               "CURLE_OBSOLETE29"},
                                                                                                            {CURLE_FTP_PORT_FAILED,          "CURLE_FTP_PORT_FAILED"},
                                                                                                            {CURLE_FTP_COULDNT_USE_REST,     "CURLE_FTP_COULDNT_USE_REST"},
                                                                                                            {CURLE_OBSOLETE32,               "CURLE_OBSOLETE32"},
                                                                                                            {CURLE_RANGE_ERROR,              "CURLE_RANGE_ERROR"},
                                                                                                            {CURLE_HTTP_POST_ERROR,          "CURLE_HTTP_POST_ERROR"},
                                                                                                            {CURLE_SSL_CONNECT_ERROR,        "CURLE_SSL_CONNECT_ERROR"},
                                                                                                            {CURLE_BAD_DOWNLOAD_RESUME,      "CURLE_BAD_DOWNLOAD_RESUME"},
                                                                                                            {CURLE_FILE_COULDNT_READ_FILE,   "CURLE_FILE_COULDNT_READ_FILE"},
                                                                                                            {CURLE_LDAP_CANNOT_BIND,         "CURLE_LDAP_CANNOT_BIND"},
                                                                                                            {CURLE_LDAP_SEARCH_FAILED,       "CURLE_LDAP_SEARCH_FAILED"},
                                                                                                            {CURLE_OBSOLETE40,               "CURLE_OBSOLETE40"},
                                                                                                            {CURLE_FUNCTION_NOT_FOUND,       "CURLE_FUNCTION_NOT_FOUND"},
                                                                                                            {CURLE_ABORTED_BY_CALLBACK,      "CURLE_ABORTED_BY_CALLBACK"},
                                                                                                            {CURLE_BAD_FUNCTION_ARGUMENT,    "CURLE_BAD_FUNCTION_ARGUMENT"},
                                                                                                            {CURLE_OBSOLETE44,               "CURLE_OBSOLETE44"},
                                                                                                            {CURLE_INTERFACE_FAILED,         "CURLE_INTERFACE_FAILED"},
                                                                                                            {CURLE_OBSOLETE46,               "CURLE_OBSOLETE46"},
                                                                                                            {CURLE_TOO_MANY_REDIRECTS,       "CURLE_TOO_MANY_REDIRECTS"},
                                                                                                            {CURLE_UNKNOWN_OPTION,           "CURLE_UNKNOWN_OPTION"},
                                                                                                            {CURLE_TELNET_OPTION_SYNTAX,     "CURLE_TELNET_OPTION_SYNTAX"},
                                                                                                            {CURLE_OBSOLETE50,               "CURLE_OBSOLETE50"},
                                                                                                            {CURLE_OBSOLETE51,               "CURLE_OBSOLETE51"},
                                                                                                            {CURLE_GOT_NOTHING,              "CURLE_GOT_NOTHING"},
                                                                                                            {CURLE_SSL_ENGINE_NOTFOUND,      "CURLE_SSL_ENGINE_NOTFOUND"},
                                                                                                            {CURLE_SSL_ENGINE_SETFAILED,     "CURLE_SSL_ENGINE_SETFAILED"},
                                                                                                            {CURLE_SEND_ERROR,               "CURLE_SEND_ERROR"},
                                                                                                            {CURLE_RECV_ERROR,               "CURLE_RECV_ERROR"},
                                                                                                            {CURLE_OBSOLETE57,               "CURLE_OBSOLETE57"},
                                                                                                            {CURLE_SSL_CERTPROBLEM,          "CURLE_SSL_CERTPROBLEM"},
                                                                                                            {CURLE_SSL_CIPHER,               "CURLE_SSL_CIPHER"},
                                                                                                            {CURLE_PEER_FAILED_VERIFICATION, "CURLE_PEER_FAILED_VERIFICATION"},
                                                                                                            {CURLE_BAD_CONTENT_ENCODING,     "CURLE_BAD_CONTENT_ENCODING"},
                                                                                                            {CURLE_LDAP_INVALID_URL,         "CURLE_LDAP_INVALID_URL"},
                                                                                                            {CURLE_FILESIZE_EXCEEDED,        "CURLE_FILESIZE_EXCEEDED"},
                                                                                                            {CURLE_USE_SSL_FAILED,           "CURLE_USE_SSL_FAILED"},
                                                                                                            {CURLE_SEND_FAIL_REWIND,         "CURLE_SEND_FAIL_REWIND"},
                                                                                                            {CURLE_SSL_ENGINE_INITFAILED,    "CURLE_SSL_ENGINE_INITFAILED"},
                                                                                                            {CURLE_LOGIN_DENIED,             "CURLE_LOGIN_DENIED"},
                                                                                                            {CURLE_TFTP_NOTFOUND,            "CURLE_TFTP_NOTFOUND"},
                                                                                                            {CURLE_TFTP_PERM,                "CURLE_TFTP_PERM"},
                                                                                                            {CURLE_REMOTE_DISK_FULL,         "CURLE_REMOTE_DISK_FULL"},
                                                                                                            {CURLE_TFTP_ILLEGAL,             "CURLE_TFTP_ILLEGAL"},
                                                                                                            {CURLE_TFTP_UNKNOWNID,           "CURLE_TFTP_UNKNOWNID"},
                                                                                                            {CURLE_REMOTE_FILE_EXISTS,       "CURLE_REMOTE_FILE_EXISTS"},
                                                                                                            {CURLE_TFTP_NOSUCHUSER,          "CURLE_TFTP_NOSUCHUSER"},
                                                                                                            {CURLE_CONV_FAILED,              "CURLE_CONV_FAILED"},
                                                                                                            {CURLE_CONV_REQD,                "CURLE_CONV_REQD"},
                                                                                                            {CURLE_SSL_CACERT_BADFILE,       "CURLE_SSL_CACERT_BADFILE"},
                                                                                                            {CURLE_REMOTE_FILE_NOT_FOUND,    "CURLE_REMOTE_FILE_NOT_FOUND"},
                                                                                                            {CURLE_SSH,                      "CURLE_SSH"},
                                                                                                            {CURLE_SSL_SHUTDOWN_FAILED,      "CURLE_SSL_SHUTDOWN_FAILED"},
                                                                                                            {CURLE_AGAIN,                    "CURLE_AGAIN"},
                                                                                                            {CURLE_SSL_CRL_BADFILE,          "CURLE_SSL_CRL_BADFILE"},
                                                                                                            {CURLE_SSL_ISSUER_ERROR,         "CURLE_SSL_ISSUER_ERROR"},
                                                                                                            {CURLE_FTP_PRET_FAILED,          "CURLE_FTP_PRET_FAILED"},
                                                                                                            {CURLE_RTSP_CSEQ_ERROR,          "CURLE_RTSP_CSEQ_ERROR"},
                                                                                                            {CURLE_RTSP_SESSION_ERROR,       "CURLE_RTSP_SESSION_ERROR"},
                                                                                                            {CURLE_FTP_BAD_FILE_LIST,        "CURLE_FTP_BAD_FILE_LIST"},
                                                                                                            {CURLE_CHUNK_FAILED,             "CURLE_CHUNK_FAILED"},
                                                                                                            {CURLE_NO_CONNECTION_AVAILABLE,  "CURLE_NO_CONNECTION_AVAILABLE"},
                                                                                                            {CURLE_SSL_PINNEDPUBKEYNOTMATCH, "CURLE_SSL_PINNEDPUBKEYNOTMATCH"},
                                                                                                            {CURLE_SSL_INVALIDCERTSTATUS,    "CURLE_SSL_INVALIDCERTSTATUS"},
                                                                                                            {CURLE_HTTP2_STREAM,             "CURLE_HTTP2_STREAM"},
                                                                                                            {CURLE_RECURSIVE_API_CALL,       "CURLE_RECURSIVE_API_CALL"},
                                                                                                            {CURLE_AUTH_ERROR,               "CURLE_AUTH_ERROR"},
                                                                                                    });

        constexpr const auto LIBCURL_URL_CODE = mapbox::eternal::map<CURLUcode, mapbox::eternal::string>({
                                                                                                                 {CURLUE_OK,                 "CURLUE_OK"},
                                                                                                                 {CURLUE_BAD_HANDLE,         "CURLUE_BAD_HANDLE"},
                                                                                                                 {CURLUE_BAD_PARTPOINTER,    "CURLUE_BAD_PARTPOINTER"},
                                                                                                                 {CURLUE_MALFORMED_INPUT,    "CURLUE_MALFORMED_INPUT"},
                                                                                                                 {CURLUE_BAD_PORT_NUMBER,    "CURLUE_BAD_PORT_NUMBER"},
                                                                                                                 {CURLUE_UNSUPPORTED_SCHEME, "CURLUE_UNSUPPORTED_SCHEME"},
                                                                                                                 {CURLUE_URLDECODE,          "CURLUE_URLDECODE"},
                                                                                                                 {CURLUE_OUT_OF_MEMORY,      "CURLUE_OUT_OF_MEMORY"},
                                                                                                                 {CURLUE_USER_NOT_ALLOWED,   "CURLUE_USER_NOT_ALLOWED"},
                                                                                                                 {CURLUE_UNKNOWN_PART,       "CURLUE_UNKNOWN_PART"},
                                                                                                                 {CURLUE_NO_SCHEME,          "CURLUE_NO_SCHEME"},
                                                                                                                 {CURLUE_NO_USER,            "CURLUE_NO_USER"},
                                                                                                                 {CURLUE_NO_PASSWORD,        "CURLUE_NO_PASSWORD"},
                                                                                                                 {CURLUE_NO_OPTIONS,         "CURLUE_NO_OPTIONS"},
                                                                                                                 {CURLUE_NO_HOST,            "CURLUE_NO_HOST"},
                                                                                                                 {CURLUE_NO_PORT,            "CURLUE_NO_PORT"},
                                                                                                                 {CURLUE_NO_QUERY,           "CURLUE_NO_QUERY"},
                                                                                                                 {CURLUE_NO_FRAGMENT,        "CURLUE_NO_FRAGMENT"}
                                                                                                         });

        constexpr const auto LIBCURL_HTTP_VERB = mapbox::eternal::map<http::HTTP_REQUEST_TYPE, mapbox::eternal::string>({
                                                                                                                                {http::HTTP_REQUEST_TYPE::GET,     "GET"},
                                                                                                                                {http::HTTP_REQUEST_TYPE::POST,    "POST"},
                                                                                                                                {http::HTTP_REQUEST_TYPE::HEAD,    "HEAD"},
                                                                                                                                {http::HTTP_REQUEST_TYPE::PUT,     "PUT"},
                                                                                                                                {http::HTTP_REQUEST_TYPE::DELETE_, "DELETE"},
                                                                                                                        });
    }
}


#endif //CPPREQUESTS_CONST_H
