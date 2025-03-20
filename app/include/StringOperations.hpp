#pragma once

#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>

namespace String {
    inline std::string formatDouble(double v, unsigned n) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(n) << v;
        return oss.str();
    }

    template<typename T>
    inline std::string padZeros(T v, unsigned n) {
        static_assert(std::is_integral<T>::value, "T must be an integral type");
        std::string s = std::to_string(v);
        bool sign_len = (s[0] == '-') ? true : false;
        std::string digits = s.substr(sign_len);
        if (digits.size() < n) {
            digits = std::string(n - digits.size(), '0') + digits;
        }
        return (sign_len ? "-" : "") + digits;
    }
}