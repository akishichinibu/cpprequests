#ifndef CPPREQUESTS__STRIP_H
#define CPPREQUESTS__STRIP_H

namespace crq::string {

    template<bool from_left, bool from_right>
    inline std::string_view _base_strip(std::string_view s, const char* delimiter_set) {
        const auto head = s.cbegin();
        const auto tail = s.cend();

        auto checker = [&](char s) -> bool { return strchr(delimiter_set, s) != nullptr; };

        auto view = std::string_view{s};

        if constexpr (from_left) {
            auto i = head;
            while (checker(*i) and i < tail) { i++; }
            const auto r = std::distance(head, i);
            if (r > 0) {
                view.remove_prefix(r);
            }
        }

        if constexpr (from_right) {
            auto j = tail;
            do { j--; } while (checker(*j) and head < j);
            const auto r = std::distance(j + 1, tail);
            if (r > 0) {
                view.remove_suffix(r);
            }
        }

        return view;
    }

    class strip {
    public:
        const char* delimiter_set;

        inline explicit strip(const char* delimiter_set) : delimiter_set(delimiter_set) {}

        inline friend auto operator|(std::string_view s, strip op) {
            return _base_strip<true, true>(s, op.delimiter_set);
        }
    };

    class lstrip {
    public:
        const char* delimiter_set;

        inline explicit lstrip(const char* delimiter_set) : delimiter_set(delimiter_set) {}

        inline friend auto operator|(std::string_view s, lstrip op) {
            return _base_strip<true, true>(s, op.delimiter_set);
        }
    };

    class rstrip {
    public:
        const char* delimiter_set;

        inline explicit rstrip(const char* delimiter_set) : delimiter_set(delimiter_set) {}

        inline friend auto operator|(std::string_view s, rstrip op) {
            return _base_strip<true, true>(s, op.delimiter_set);
        }
    };
}

#endif //CPPREQUESTS__STRIP_H
