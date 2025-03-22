#pragma once
#include <fstream>
#include <nlohmann/json.hpp>

class Settings {
    private:
        static const std::string filename;
    public:
        static void save(std::string key, int x);
        static int read(std::string key);
};