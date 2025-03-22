#include "objects/WheelsScript.h"

WheelsScript::WheelsScript(CarMovement& movement_script, std::string model, float off_left, float off_right, float off_forw, float off_back, float off_up) 
: movement_script(movement_script), model(model), off_left(off_left), off_right(off_right), off_forw(off_forw), off_back(off_back), off_up(off_up)
{}

void WheelsScript::Init() {
    if (model == "") {
        if (gameObject->GetModelName() == "f1car") {
            const_cast<std::string&>(model) = "tire_f1car";
            const_cast<float&>(off_left) = 0.6f;
            const_cast<float&>(off_right) = 0.6f;
            const_cast<float&>(off_forw) = 1.4f;
            const_cast<float&>(off_back) = 1.1f;
            const_cast<float&>(off_up) = 0.05f;
        } else if (gameObject->GetModelName() == "racing_car") {
            const_cast<std::string&>(model) = "3x3_tire_1";
            const_cast<float&>(off_left) = 0.75f;
            const_cast<float&>(off_right) = 0.75f;
            const_cast<float&>(off_forw) = 1.4f;
            const_cast<float&>(off_back) = 0.8f;
            const_cast<float&>(off_up) = 0.05f;
        } else if (gameObject->GetModelName() == "hotrod") {
            const_cast<std::string&>(model) = "3x3_tire_2";
            const_cast<float&>(off_left) = 0.75f;
            const_cast<float&>(off_right) = 0.75f;
            const_cast<float&>(off_forw) = 0.85f;
            const_cast<float&>(off_back) = 1.4f;
            const_cast<float&>(off_up) = 0.05f;
        } else if (gameObject->GetModelName() == "pickup") {
            const_cast<std::string&>(model) = "pickup_tire_1";
            const_cast<float&>(off_left) = 0.75f;
            const_cast<float&>(off_right) = 0.75f;
            const_cast<float&>(off_forw) = 1.2f;
            const_cast<float&>(off_back) = 1.1f;
            const_cast<float&>(off_up) = 0.3f;
        }
    }

    for (int i = 0; i < wheels.size(); ++i)
        wheels[i] = new GameObject(model);
}

void WheelsScript::Update() {}
void WheelsScript::LateUpdate() {
    for (int i = 0; i < wheels.size(); ++i) {
        const auto& x = wheels[i];
        float move_rot = x->transform->rotation.y;
        x->transform->MoveTo(gameObject->transform->position);
        x->transform->RotateTo(gameObject->transform->rotation);
        x->transform->Translate(glm::vec3(((i&1) ? off_forw : -off_back), ((i&2) ? -off_left : off_right), off_up));
        if (i&1) {
            x->transform->Rotate(glm::vec3(0.0f, 0.0f, movement_script.axleAngle));
        }
        move_rot += -movement_script.actSpeed*movement_script.forces.x*Time::deltaTime*30.0f;
        x->transform->Rotate(glm::vec3(0.0f, move_rot, 0.0f));
    }
}

void WheelsScript::OnDestroy() {
    for (int i = 0; i < wheels.size(); ++i)
        delete wheels[i];
}