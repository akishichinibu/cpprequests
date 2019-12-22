#ifndef CPPREQUESTS_UTILS_H
#define CPPREQUESTS_UTILS_H

#include <cstring>
#include <map>
#include <algorithm>

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "requests/const.h"

#define metaCONCAT(x, y) x ## y
#define CONCAT(x, y) metaCONCAT(x, y)

#define CONST_REF_PROPERTY(fun, member, type) [[nodiscard]] inline auto fun() const -> const type& { return this->member; }
#define REF_PROPERTY(fun, member, type) [[nodiscard]] inline auto fun() -> type& { return this->member; }
#define PRIMARY_PROPERTY(fun, member, type) [[nodiscard]] inline auto fun() const -> type { return this->member; }

namespace crq::string {

    inline std::string& upper(std::string &s) {
        std::transform(s.cbegin(), s.cend(), s.begin(), ::toupper);
        return s;
    }

    inline std::string upper(const std::string &s) {
        std::string buf;
        buf.resize(s.size());
        std::transform(s.cbegin(), s.cend(), buf.begin(), ::toupper);
        return buf;
    }

    inline std::string& lower(std::string &s) {
        std::transform(s.cbegin(), s.cend(), s.begin(), ::tolower);
        return s;
    }

    inline std::string lower(const std::string &s) {
        std::string buf;
        buf.resize(s.size());
        std::transform(s.cbegin(), s.cend(), buf.begin(), ::tolower);
        return buf;
    }

    inline std::pair<const char*, const char*> char_strip(const char* start, const char* end) {
        auto head = start;
        auto tail = end;

        auto checker = [](char s) -> bool { return (s == '\n') || (s == '\r') || (s == ' '); };

        while (checker(*head)) { head++; }
        do { tail--; } while (checker(*tail));

        return {head, tail + 1};
    }
}

#endif //CPPREQUESTS_UTILS_H
