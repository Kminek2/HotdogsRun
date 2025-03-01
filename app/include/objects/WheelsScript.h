#pragma once
#include "ObjectScript.h"
#include "GameObject.h"
#include "objects/CarMovement.h"
#include <array>

class WheelsScript :
    public ObjectScript {
private:
    CarMovement& movement_script;
    const float off_left;
    const float off_right;
    const float off_forw;
    const float off_back;
    const float off_up;
    const std::string model;
    std::array<GameObject*, 4> wheels;
public:
    WheelsScript(CarMovement& movement_script, std::string model, float off_left, float off_right, float off_forw, float off_back, float off_up = 0.0f);
    void Init() override;
    void Update() override;
    void LateUpdate() override;
    void OnDestroy() override;
};