#pragma once

#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>
#include <fstream>
#include <vector>
#include <functional>

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

	template<typename T>
	inline std::string getSuffix(T v) {
		static_assert(std::is_integral_v<T>, "T must be an integral type");

		if (v / 10 == 1) return "th";

		switch (v % 10) {
			case 1:  return "st";
			case 2:  return "nd";
			case 3:  return "rd";
			default: return "th";
		}
	}

	inline std::vector<std::string> getFile(std::string file) {
		std::ifstream input(file);

		std::string line;
		std::vector<std::string> text;

		while (getline(input, line))
			text.push_back(line);

		input.close();
		return text;
	}

	inline std::string cropString(std::string s, unsigned n, std::string indicator = "...") {
		if (n >= s.size()) return s;
		return s.substr(0, n) + indicator;
	}

	inline size_t getHash(std::string s) {
		return std::hash<std::string>{}(s);
	}
}