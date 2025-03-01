#pragma once
#include "ObjectScript.h"
#include "GameObject.h"
#include "Input.h"
#include "objects/CarMovement.h"

class CarInputs :
    public ObjectScript {
private:
    CarMovement& script;
    const uint16_t forward_key;
    const uint16_t backwards_key;
    const uint16_t left_turn_key;
    const uint16_t right_turn_key;
    const uint16_t hand_break_key;

public:
    CarInputs(CarMovement& script, uint16_t forward_key = GLFW_KEY_W, uint16_t backwards_key = GLFW_KEY_S, uint16_t left_turn_key = GLFW_KEY_A, uint16_t right_turn_key = GLFW_KEY_D, uint16_t hand_break_key = GLFW_KEY_SPACE);
    void Init() override;
    void EarlyUpdate() override;
    void Update() override;
    void LateUpdate() override;
    void OnDestroy() override;
};