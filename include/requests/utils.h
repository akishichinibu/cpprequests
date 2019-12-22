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

#define _ERROR_CHECK(expr, err_map, ok, exception) auto CONCAT(__res__, __LINE__) = expr; auto CONCAT(__error__, __LINE__) = err_map.at(CONCAT(__res__, __LINE__)); if (CONCAT(__res__, __LINE__) != ok) { spdlog::error("[{0:d}][{1:s}]{2}", CONCAT(__res__, __LINE__), std::get<0>(CONCAT(__error__, __LINE__)), std::get<1>(CONCAT(__error__, __LINE__))); throw exception(std::get<0>(CONCAT(__error__, __LINE__))); }

#define LIBCURL_ERROR_CHECK(expr) _ERROR_CHECK(expr, crq::curl::LIBCURL_CODE, CURLE_OK, std::runtime_error)

#define PRIMARY_TYPE(name) decltype(name)
#define REF_TYPE(name) decltype(name)&
#define CONST_REF_TYPE(name) const decltype(name)&

#define _CONST_PROPERTY(fun, member, type) [[nodiscard]] inline auto fun() const -> const type { return this->member; }
#define _PROPERTY(fun, member, type) [[nodiscard]] inline auto fun() -> type { return this->member; }

#define CONST_REF_PROPERTY(fun, member, type) [[nodiscard]] inline auto fun() const -> const type& { return this->member; }
#define REF_PROPERTY(fun, member, type) [[nodiscard]] inline auto fun() -> type& { return this->member; }
#define PRIMARY_PROPERTY(fun, member, type) [[nodiscard]] inline auto fun() const -> type { return this->member; }

template<typename K, typename V>
inline V get_or_throw(const std::map<K, V> dict, const K& key) {
    const auto& item = dict.find(key);

    if (item == dict.end()) {
        throw std::runtime_error("Key error. ");
    } else {
        return (*item).second;
    }
}

inline std::string upper(const std::string& s) {
    std::string buf;
    buf.resize(s.size());
    std::transform(s.cbegin(), s.cend(), buf.begin(), ::toupper);
    return buf;
}

inline std::string lower(const std::string& s) {
    std::string buf;
    buf.resize(s.size());
    std::transform(s.cbegin(), s.cend(), buf.begin(), ::tolower);
    return buf;
}

#endif //CPPREQUESTS_UTILS_H
