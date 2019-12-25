#ifndef CPPREQUESTS_STRUCTURES_H
#define CPPREQUESTS_STRUCTURES_H

namespace crq {

    struct CaseInsensitiveComparator {
        inline bool operator()(const std::string &A, const std::string &B) const {
            return strcasecmp(A.c_str(), B.c_str()) < 0;
        }
    };

    using HeaderMap = std::map<std::string, std::string, CaseInsensitiveComparator>;

    using CURLUptrType = std::unique_ptr<CURLU, decltype(curl_url_cleanup) *>;

    using CURLCptrType = std::unique_ptr<char, decltype(curl_free) *>;

    using ParamsMap = std::map<std::string, std::string>;
}

#endif //CPPREQUESTS_STRUCTURES_H
