#ifndef CPPREQUESTS_UTILS_H
#define CPPREQUESTS_UTILS_H

#include <cstring>
#include <map>
#include <algorithm>
#include <numeric>

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "requests/const.h"


#define _EXPOSE_GETTER(expose, member, _type) \
[[nodiscard]] inline auto expose() const -> typename std::add_lvalue_reference_t<typename std::add_const_t<_type>> { \
    return this->member; \
}

#define EXPOSE_REF_GETTER(expose, member, _type) \
[[nodiscard]] inline auto expose() -> typename std::add_lvalue_reference_t<_type> { \
    return this->member; \
}

#define _EXPOSE_SETTER(expose, member, _type) \
inline auto expose(typename std::add_rvalue_reference_t<typename std::add_const_t<_type>> __##expose) -> decltype(*this)& { \
    this->member = __##expose; \
    return *this; \
} \
inline auto expose(typename std::add_lvalue_reference_t<typename std::add_const_t<_type>> __##expose) -> decltype(*this)& { \
    this->member = __##expose; \
    return *this; \
}

#define NOT_ALLOW_MODIFY_PROPERTY(expose, member, type) _EXPOSE_GETTER(expose, member, type)

#define ALLOW_MODIFY_PROPERTY(expose, member, type) _EXPOSE_GETTER(expose, member, type) \
\
_EXPOSE_SETTER(expose, member, type)


namespace crq::utils {

    std::string _parse_content_type_header(const std::string& header) {

    }

    inline std::string get_encoding_from_headers(const HeaderMap& headers) {

        if (headers.find("content-type") == headers.end()) {
            return "";
        }

        const auto content_type = headers.at("content-type");

//            def _parse_content_type_header(header):
//            """Returns content type and parameters from given header
//
//            :param header: string
//            :return: tuple containing content type and dictionary of
//                    parameters
//            """
//
//            tokens = header.split(';')
//            content_type, params = tokens[0].strip(), tokens[1:]
//            params_dict = {}
//            items_to_strip = "\"' "
//
//            for param in params:
//            param = param.strip()
//            if param:
//                key, value = param, True
//            index_of_equals = param.find("=")
//            if index_of_equals != -1:
//            key = param[:index_of_equals].strip(items_to_strip)
//            value = param[index_of_equals + 1:].strip(items_to_strip)
//            params_dict[key.lower()] = value
//            return content_type, params_dict


    }

    inline std::string default_user_agent(const std::string& name = "cpp-requests") {
        return name + "/" + "1.0.0";
    }

    inline crq::HeaderMap default_headers() {
        return {
                {"User-Agent",      default_user_agent()},
                {"Accept-Encoding", "gzip, deflate"},
                {"Accept",          "*/*"},
                {"Accept-Language", "zh-CN,zh;q=0.8"},
                {"Connection",      "keep-alive"}
        };
    }

    template<char KV, char KS>
    inline std::string params_encode(const ParamsMap& params) {
        using ele = std::map<std::string, std::string>;

        auto size = 0;

        for (auto& r: params) {
            size += r.first.size() + r.second.size() + 2;
        }

        std::string buf;
        buf.reserve(size);

        auto head = buf.begin();

        for (auto& r: params) {
            std::copy(r.first.cbegin(), r.first.cend(), head);
            head += r.first.size();

            *head = KV;
            head += 1;

            std::copy(r.second.cbegin(), r.second.cend(), head);
            head += r.second.size();

            *head = KS;
            head += 1;
        }

        buf.erase(buf.end() - 1);
        return buf;
    }

    inline crq::HeaderMap merge_headers(const crq::HeaderMap& new_header,
                                        const crq::HeaderMap& inner_header) {
        auto buf = HeaderMap(inner_header.begin(), inner_header.end());

        for (auto& item: new_header) {
            buf[item.first] = item.second;
        }

        return buf;
    }
}

#endif //CPPREQUESTS_UTILS_H
