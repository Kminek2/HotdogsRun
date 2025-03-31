#pragma once
#include "ObjectScript.h"
#include "GameObject.h"
#include "Input.h"
#include "objects/CarMovement.h"

class CarInputs :
    public ObjectScript {
private:
    CarMovement& script;

    // this is so ugly
    // im so tempted to add an std::array // you should have. im having a nervous breakdown here (it's 1am and i haven't slept for like 20h)
                                          // also, there's this thing called enums, you know?
    const uint16_t forward_key;
    const uint16_t backwards_key;
    const uint16_t left_turn_key;
    const uint16_t right_turn_key;
    const uint16_t hand_break_key;
    const uint16_t nitro_key;
    const uint16_t bomb_key;

public:
    CarInputs(CarMovement& script, uint16_t forward_key = GLFW_KEY_W, uint16_t backwards_key = GLFW_KEY_S, uint16_t left_turn_key = GLFW_KEY_A, uint16_t right_turn_key = GLFW_KEY_D, uint16_t hand_break_key = GLFW_KEY_SPACE, uint16_t nitro_key = GLFW_KEY_F, uint16_t bomb_key = GLFW_KEY_E);
    void Init() override;
    void EarlyUpdate() override;
    void Update() override;
    void LateUpdate() override;
    void OnDestroy() override;

    CarInputs* copy() override { return new CarInputs(*this); }
};