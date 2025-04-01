#include "objects/PowerUps/Bomb.hpp"
#include "objects/CarMovement.h"

std::vector<GameObject*> Bomb::cars;

void Bomb::Init() {
    timer = 3.0f;
}

void Bomb::Update() {
    if (gameObject->transform->position.x > 999.0f)
        return;

    timer -= Time::deltaTime;
    timer = std::max(timer, 0.0f);
    if (timer == 0.0f) {
        for (auto& x : cars) {
            float dist = glm::distance(x->transform->position, gameObject->transform->position);
            float dist_pr = dist/20.0f;
            if (dist_pr < 1.0f) {
                x->cm->forces.x = -x->cm->forces.x;
                x->cm->actSpeed += dist_pr * 100.0f;
                x->cm->gripMult = x->transform->position - gameObject->transform->position;
            }
        }
        gameObject->transform->MoveTo(glm::vec3(1000.0f));
    }
}

void Bomb::OnDestroy() {}
