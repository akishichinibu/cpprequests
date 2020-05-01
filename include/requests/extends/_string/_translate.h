#ifndef CPPREQUESTS__TRANSLATE_H
#define CPPREQUESTS__TRANSLATE_H

namespace crq::string {

    template<typename _Handler>
    inline std::string& _translate(std::string& s, _Handler H) {
        std::transform(s.cbegin(), s.cend(), s.begin(), H);
        return s;
    }

    class upper {
    public:
        inline friend std::string& operator|(std::string& s, upper op) {
            return _translate(s, [](char a) -> char { return std::toupper(a); });
        }

        inline friend std::string operator|(const std::string& s, upper op) {
            auto lv_copy = std::string(s);
            return _translate(lv_copy, [](char a) -> char { return std::toupper(a); });
        }
    };

    class lower {
    public:
        inline friend auto operator|(std::string& s, lower op) {
            return _translate(s, [](char a) -> char { return std::tolower(a); });
        }

        inline friend std::string operator|(const std::string& s, lower op) {
            auto lv_copy = std::string(s);
            return _translate(lv_copy, [](char a) -> char { return std::tolower(a); });
        }
    };
}

#endif //CPPREQUESTS__TRANSLATE_H
