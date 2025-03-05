#include "objects/CarInputs.h"

CarInputs::CarInputs(CarMovement& script, uint16_t forward_key, uint16_t backwards_key, uint16_t left_turn_key, uint16_t right_turn_key, uint16_t hand_break_key, uint16_t nitro_key) 
: script(script), forward_key(forward_key), backwards_key(backwards_key), left_turn_key(left_turn_key), right_turn_key(right_turn_key), hand_break_key(hand_break_key), nitro_key(nitro_key)
{}

void CarInputs::Init() {}

void CarInputs::EarlyUpdate() {
    if (Input::getKeyPressed(forward_key))
        script.goForward();
    if (Input::getKeyPressed(backwards_key))
        script.goBackwards();
    if (Input::getKeyPressed(left_turn_key))
        script.makeLeftTurn();
    if (Input::getKeyPressed(right_turn_key))
        script.makeRightTurn();
    if (Input::getKeyPressed(hand_break_key))
        script.useHandBreak();
    if (Input::getKeyClicked(nitro_key))
        script.useNitro();
}

void CarInputs::Update() {}
void CarInputs::LateUpdate() {}
void CarInputs::OnDestroy() {}