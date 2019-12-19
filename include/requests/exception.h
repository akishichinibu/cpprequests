#ifndef CPPREQUESTS_EXCEPTION_H
#define CPPREQUESTS_EXCEPTION_H

#include <stdexcept>
#include <exception>

namespace crq {

    class RequestException : std::runtime_error {
        // There was an ambiguous exception that occurred while handling your
        // request.

        RequestException() : std::runtime_error("") {
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


    class MissingSchema : RequestException, std::invalid_argument {
        // The URL schema (e.g. http or https) is missing.
    };


}






/*

class RequestException(IOError):
    """There was an ambiguous exception that occurred while handling your
    request.
    """

    def __init__(self, *args, **kwargs):
        """Initialize RequestException with `request` and `response` objects."""
        response = kwargs.pop('response', None)
        self.response = response
        self.request = kwargs.pop('request', None)
        if (response is not None and not self.request and
                hasattr(response, 'request')):
            self.request = self.response.request
        super(RequestException, self).__init__(*args, **kwargs)


class ReadTimeout(Timeout):
    """The server did not send any data in the allotted amount of time."""


class URLRequired(RequestException):
    """A valid URL is required to make a request."""


class TooManyRedirects(RequestException):
    """Too many redirects."""


class MissingSchema(RequestException, ValueError):
    """The URL schema (e.g. http or https) is missing."""


class InvalidSchema(RequestException, ValueError):
    """See defaults.py for valid schemas."""


class InvalidURL(RequestException, ValueError):
    """The URL provided was somehow invalid."""


class InvalidHeader(RequestException, ValueError):
    """The header value provided was somehow invalid."""


class InvalidProxyURL(InvalidURL):
    """The proxy URL provided is invalid."""


class ChunkedEncodingError(RequestException):
    """The server declared chunked encoding but sent an invalid chunk."""


class ContentDecodingError(RequestException, BaseHTTPError):
    """Failed to decode response content"""


class StreamConsumedError(RequestException, TypeError):
    """The content for this response was already consumed"""


class RetryError(RequestException):
    """Custom retries logic failed"""


class UnrewindableBodyError(RequestException):
    """Requests encountered an error when trying to rewind a body"""

# Warnings


class RequestsWarning(Warning):
    """Base warning for Requests."""
    pass


class FileModeWarning(RequestsWarning, DeprecationWarning):
    """A file was opened in text mode, but Requests determined its binary length."""
    pass


class RequestsDependencyWarning(RequestsWarning):
    """An imported dependency doesn't match the expected version range."""
    pass
*/


#endif //CPPREQUESTS_EXCEPTION_H
