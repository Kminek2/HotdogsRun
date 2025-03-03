#include "objects/WheelsScript.h"

WheelsScript::WheelsScript(CarMovement& movement_script, std::string model, float off_left, float off_right, float off_forw, float off_back, float off_up) 
: movement_script(movement_script), model(model), off_left(off_left), off_right(off_right), off_forw(off_forw), off_back(off_back), off_up(off_up)
{}

void WheelsScript::Init() {
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

void WheelsScript::OnDestroy() {}