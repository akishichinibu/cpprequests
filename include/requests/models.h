#ifndef CPPREQUESTS_MODELS_H
#define CPPREQUESTS_MODELS_H

#include <map>


namespace crq {

    class PreparedRequest;

    class Request {

    public:
        typedef enum {
            GET,
            POST,
            PUT,
            DELETE,
            TRACE,
            HEAD,
            OPTIONS,
            CONNECT
        } request_type;

        request_type method;
        std::string url;
        std::map<std::string, std::string> _headers;
        std::map<std::string, std::string> _params;

        Request(request_type __method, std::string __url):
        method(__method),
        url(std::move(__url)) {}

        Request& headers(std::map<std::string, std::string> __headers) {
            this->_headers = std::move(__headers);
            return *this;
        }

        Request& params(std::map<std::string, std::string> __params) {
            this->_params = std::move(__params);
            return *this;
        }
    };

    class PreparedRequest {
        // The fully mutable :class:`PreparedRequest <PreparedRequest>` object,
        // containing the exact bytes that will be sent to the server.
        // Generated from either a :class:`Request <Request>` object or manually.

    public:
        std::string _method;

        PreparedRequest() = default;


        void prepare_method(std::string __method) {
            this->_method = std::move(__method);
        }
    };
}

#endif //CPPREQUESTS_MODELS_H
