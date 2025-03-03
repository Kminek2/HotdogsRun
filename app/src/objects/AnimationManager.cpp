#include "objects/AnimationManager.h"

AnimationManager::AnimationManager(std::vector<data> animation_queue) {
    this->animation_queue = animation_queue;
    last_animation = nullptr;
    id = 0;
}

void AnimationManager::Init() {}

void AnimationManager::Update() {
    if (id < animation_queue.size() && (last_animation == nullptr || last_animation->hasEnded())) {
        last_animation = new CinematicCamera(animation_queue[id].start, animation_queue[id].end, animation_queue[id].animation_time, animation_queue[id].cur_offset, false);
        gameObject->AddScript(last_animation);
        ++id;
    }
}

void AnimationManager::OnDestroy() {}

void AnimationManager::addToQueue(data animation) {
    animation_queue.push_back(animation);
}