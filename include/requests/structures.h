#ifndef CPPREQUESTS_STRUCTURES_H
#define CPPREQUESTS_STRUCTURES_H

namespace crq {

    struct CaseInsensitiveComparator {
        inline bool operator()(const std::string &A, const std::string &B) const {
            return strcasecmp(A.c_str(), B.c_str()) < 0;
        }
    };

    using HeaderMap = std::map<std::string, std::string, CaseInsensitiveComparator>;
}

#endif //CPPREQUESTS_STRUCTURES_H
