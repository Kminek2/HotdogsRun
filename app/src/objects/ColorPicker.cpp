#include "objects/ColorPicker.hpp"
#include "GameObject.h"
#include <complex>
#include <iostream>
#include <cmath>

const float ColorPicker::PI = std::acos(-1);

const std::vector<std::vector<glm::vec3>> default_colors = {
    {
		glm::vec3(0.9686274509803922f, 0.37254901960784315f, 0.17647058823529413f),
		glm::vec3(0.8666666666666667f, 0.8666666666666667f, 0.8666666666666667f),
		glm::vec3(0.43137254901960786f, 0.43137254901960786f, 0.43137254901960786f)
	},
	{
		glm::vec3(0.26666666666666666f, 0.26666666666666666f, 0.26666666666666666f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f)
	},
	{
		glm::vec3(0.9333333333333333f, 0.0f, 0.0f),
		glm::vec3(0.3333333333333333f, 0.3333333333333333f, 0.3333333333333333f),
		glm::vec3(0.9333333333333333f, 0.9333333333333333f, 0.9333333333333333f)
	},
	{
		glm::vec3(0.0f, 0.4f, 0.8f),
		glm::vec3(0.9333333333333333f, 0.9333333333333333f, 0.9333333333333333f),
		glm::vec3(0.06666666666666667f, 0.06666666666666667f, 0.06666666666666667f)
	}
};


const std::vector<std::string> colors = {"primary", "secondary", "other"};

ColorPicker::ColorPicker() {
    car = nullptr;
}

ColorPicker::ColorPicker(GameObject* car) {
    this->car = car;
}

void ColorPicker::add_picker(Sprite *picker, const std::string part) { pickers.push_back({picker, part}); }

void ColorPicker::update() {
	if (!Input::getKeyPressed(GLFW_MOUSE_BUTTON_1))
		return;

	for (auto &pick : pickers)
		if (update_picker(pick))
			break;
}

bool ColorPicker::update_picker(std::pair<Sprite *, std::string> picker) {
	glm::vec2 mouse_pos = Input::mousePercPos;
	glm::vec2 picker_pos = picker.first->rectTransform->position;
	picker_pos += glm::vec2(1.0f);
	picker_pos *= glm::vec2(0.5f);
	picker_pos.y = 1.0f - picker_pos.y;

	std::complex<float> diff = std::complex<float>((picker_pos.x - mouse_pos.x) * picker.first->rectTransform->scale.y,
						       (picker_pos.y - mouse_pos.y) * picker.first->rectTransform->scale.x);


    float dist = std::abs(diff);
    if (dist > 0.01f)
        return false;

    float ang = std::arg(diff);
    ang += PI;
    ang /= 2.0f*PI;
    ang += 0.25f;
    if (ang > 1.0f)
        ang -= 1.0f;
    
    glm::vec3 new_color = hsb_to_rgb(glm::vec3(ang, 1.0f, 1.0f));

    Settings::save(picker.second+"_r", new_color.x*100000.0f);
    Settings::save(picker.second+"_g", new_color.y*100000.0f);
    Settings::save(picker.second+"_b", new_color.z*100000.0f);

    update_car();

	return true;
}

void ColorPicker::close_all_pickers() {
	for (auto &pick : pickers) {
		delete pick.first;
	}
	pickers.clear();
}

ColorPicker::~ColorPicker() { close_all_pickers(); }

glm::vec3 ColorPicker::hsb_to_rgb(const glm::vec3 hsb) {
    glm::vec3 rgb;

    float h = hsb.x;
    float s = hsb.y;
    float b = hsb.z;

    if (s == 0.0f) {
        rgb = glm::vec3(b, b, b);
    } else {
        float hue = h * 6.0f;
        int sector = static_cast<int>(std::floor(hue));
        float fraction = hue - sector;
        float p = b * (1.0f - s);
        float q = b * (1.0f - s * fraction);
        float t = b * (1.0f - s * (1.0f - fraction));

        switch (sector % 6) {
            case 0: rgb = glm::vec3(b, t, p); break;
            case 1: rgb = glm::vec3(q, b, p); break;
            case 2: rgb = glm::vec3(p, b, t); break;
            case 3: rgb = glm::vec3(p, q, b); break;
            case 4: rgb = glm::vec3(t, p, b); break;
            case 5: rgb = glm::vec3(b, p, q); break;
        }
    }

    return rgb;
}

void ColorPicker::update_car() {
    int car_type = Settings::read("model_choosen").value_or(0);
    
    for (int i = 0; i < colors.size(); ++i) {
        int ri = Settings::read(colors[i]+"_r").value_or(-1);
        int gi = Settings::read(colors[i]+"_g").value_or(-1);
        int bi = Settings::read(colors[i]+"_b").value_or(-1);
        if (ri == -1 || gi == -1 || bi == -1)
            continue;
        std::cout << i << '\n';
        float r = static_cast<float>(ri)/100000.0f;
        float g = static_cast<float>(bi)/100000.0f;
        float b = static_cast<float>(gi)/100000.0f;
        std::cout << r << ' ' << g << ' ' << b << '\n';
        car->AddColorChange(default_colors[car_type][i], glm::vec3(r,g,b));
    }
}