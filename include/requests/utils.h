#ifndef CPPREQUESTS_UTILS_H
#define CPPREQUESTS_UTILS_H

#include <string_view>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <cstring>
#include <map>

#define metaCONCAT(x, y) x ## y
#define CONCAT(x, y) metaCONCAT(x, y)

#define LIBCURL_ERROR_CHECK(expr) auto CONCAT(__res__, __LINE__) = expr; auto CONCAT(__error__, __LINE__) = LIBCURL_CODE.at(CONCAT(__res__, __LINE__)); if (CONCAT(__res__, __LINE__) != CURLE_OK) { spdlog::error("[{0:d}][{1:s}]{2}", CONCAT(__res__, __LINE__), std::get<0>(CONCAT(__error__, __LINE__)), std::get<1>(CONCAT(__error__, __LINE__))); throw std::runtime_error(std::get<0>(CONCAT(__error__, __LINE__))); }

namespace crq {

    struct CaseInsensitiveComparator {
        inline bool operator()(const std::string& A, const std::string& B) const {
            return strcasecmp(A.c_str(), B.c_str()) < 0;
        }
    };

    using HeaderMap = std::map<std::string, std::string, CaseInsensitiveComparator>;

    inline std::string default_user_agent(const std::string& name="cpp-requests") {
        return fmt::format("{0}/{1}", name, "1.0.0");
    }


    inline HeaderMap default_headers() {
        return {
            {"User-Agent", default_user_agent()},
//            {"Accept-Encoding", "gzip, deflate, sdch"},
            {"Accept", "*/*"},
            {"Accept-Language", "zh-CN,zh;q=0.8"},
            {"Connection", "keep-alive"}
        };
    }
}

#endif //CPPREQUESTS_UTILS_H
