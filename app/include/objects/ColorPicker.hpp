#pragma once

#include "Sprite.h"
#include <vector>
#include "Input.h"

class ColorPicker {
    private:
        static const float PI;
        GameObject* car;
        std::vector<std::pair<Sprite*, std::string>> pickers;
        
        bool update_picker(std::pair<Sprite*, std::string> picker);
        glm::vec3 hsb_to_rgb(const glm::vec3 hsb);
        void update_car();
    public:
        ColorPicker();
        ColorPicker(GameObject* car);
        ~ColorPicker();
        
        void add_picker(Sprite *picker, const std::string part);
        void update();
        void close_all_pickers();
};