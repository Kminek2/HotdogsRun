#include "Settings.h"

const std::string Settings::filename = "settings.json";

void Settings::save(std::string key, int x) {
    std::ifstream inp(filename);
    nlohmann::json j;

    if (inp.is_open()) {
        j = nlohmann::json::parse(inp);
        inp.close();
        j[key] = x;
    }

    std::ofstream out(filename);
    out << std::setw(4) << j << std::endl;
    out.close();
}

std::pair<bool, std::any> Settings::read(std::string key) {
    std::ifstream inp(filename);

    if (!inp.is_open())
        return {false, 0};

    nlohmann::json j = nlohmann::json::parse(inp);
    inp.close();
    
    auto found = j.find(key);

    if (found == j.end())
        return {false, 0};

    return {true, j[key]};
}