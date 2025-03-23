#pragma once
#include <fstream>
#include <nlohmann/json.hpp>

class Settings {
    private:
        static const std::string filename;
        static nlohmann::json json_data;
        static bool initialized;
        static void initialize();
    public:
        static void save(std::string key, int x);
        static std::optional<int> read(std::string key);
};