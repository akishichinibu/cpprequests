#ifndef CPPREQUESTS_EXCEPTION_H
#define CPPREQUESTS_EXCEPTION_H

#include <stdexcept>
#include <exception>

namespace crq {

    namespace exception {

        class RequestException : std::runtime_error {
            // There was an ambiguous exception that occurred while handling your request.
        public:
            explicit RequestException(const std::string& content) : std::runtime_error(content) {
                // Initialize RequestException with `request` and `response` objects.
            }
        };

        class HTTPError : RequestException {
        };

        class ConnectionError : RequestException {
        };

        class ProxyError : ConnectionError {
        };

        class SSLError : ConnectionError {
        };

        class Timeout : RequestException {
            // The request timed out.
            // Catching this error will catch both
            // :exc:`~requests.exceptions.ConnectTimeout` and
            // :exc:`~requests.exceptions.ReadTimeout` errors.

        };

        class ConnectTimeout : ConnectionError, Timeout {
        };


        class ReadTimeout : Timeout {
            // The server did not send any data in the allotted amount of time.
        };

        class URLRequired : RequestException {
            // A valid URL is required to make a request.
        };


        class TooManyRedirects : RequestException {
            // Too many redirects.
        };


        class MissingSchema : RequestException {
            // The URL schema (e.g. http or https) is missing.
        public:
            explicit MissingSchema(): RequestException("MissingSchema") {}
        };

        class InvalidSchema : RequestException {
            // See defaults.py for valid schemas.
        };

        class InvalidURL : RequestException {
            // The URL provided was somehow invalid.
        public:
            explicit InvalidURL(): RequestException("InvalidURL") {}

            explicit InvalidURL(const std::string& content): RequestException(content) {}
        };
    }

}

#endif //CPPREQUESTS_EXCEPTION_H
