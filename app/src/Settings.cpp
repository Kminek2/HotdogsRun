#include "Settings.h"

const std::string Settings::filename = "settings.json";
nlohmann::json Settings::json_data = nlohmann::json();
bool Settings::initialized = false;

void Settings::save(std::string key, int x) {
    if (!initialized)
        initialize();

    json_data[key] = x;

    std::ofstream out(filename);
    out << std::setw(4) << json_data << std::endl;
    out.close();
}

std::optional<int> Settings::read(std::string key) {
    if (!initialized)
        initialize();

    auto found = json_data.find(key);

    if (found == json_data.end())
        return {};

    return json_data[key].get<int>();
}

void Settings::initialize() {
    std::ifstream inp(filename);

    if (!inp.is_open())
        return;

    json_data = nlohmann::json::parse(inp);
    inp.close();

    initialized = true;
}