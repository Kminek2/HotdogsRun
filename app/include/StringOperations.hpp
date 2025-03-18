#include <iomanip>
#include <sstream>
#include <string>

namespace String {
    std::string formatDouble(double v, unsigned n) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(n) << v;
        return oss.str();
    }
}