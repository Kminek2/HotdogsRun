#include "objects/PowerUps/Bomb.hpp"
#include "AudioSource3d.h"
#include "GameObject.h"
#include "objects/CarMovement.h"

std::vector<GameObject*> Bomb::cars;

void Bomb::Init() {
    sound_obj = new GameObject();
    sound_obj->surface_type = NEVER_COLLIDE;
    explosion_sound = new AudioSource3d(sound_obj, "explosion", Settings::read("volume").value_or(50)/100.0f);
    timer = 3.0f;
}

void Bomb::Update() {
    if (gameObject->transform->position.x > 999.0f)
        return;
    
    timer -= Time::deltaTime;
    timer = std::max(timer, 0.0f);
    if (timer == 0.0f) {
        explosion_sound->PlayTrack(false);
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
    } else if (timer > 0.0f) {
        sound_obj->transform->MoveTo(gameObject->transform->position);
    }
}

void Bomb::OnDestroy() {}
