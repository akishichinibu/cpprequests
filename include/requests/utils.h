#ifndef CPPREQUESTS_UTILS_H
#define CPPREQUESTS_UTILS_H

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <cstring>
#include <map>

#define metaCONCAT(x, y) x ## y
#define CONCAT(x, y) metaCONCAT(x, y)

#define LIBCURL_ERROR_CHECK(expr) auto CONCAT(__res__, __LINE__) = expr; auto CONCAT(__error__, __LINE__) = crq::curl::LIBCURL_CODE.at(CONCAT(__res__, __LINE__)); if (CONCAT(__res__, __LINE__) != CURLE_OK) { spdlog::error("[{0:d}][{1:s}]{2}", CONCAT(__res__, __LINE__), std::get<0>(CONCAT(__error__, __LINE__)), std::get<1>(CONCAT(__error__, __LINE__))); throw std::runtime_error(std::get<0>(CONCAT(__error__, __LINE__))); }


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


#endif //CPPREQUESTS_UTILS_H
