#include "objects/AnimationManager.h"

AnimationManager::AnimationManager(std::vector<data> animation_queue) {
    this->animation_queue = animation_queue;
    last_animation = nullptr;
    id = 0;
    disabled_skips = false;
}

void AnimationManager::Init() {}

void AnimationManager::EarlyUpdate() {
    if (id < animation_queue.size() && (last_animation == nullptr || last_animation->hasEnded())) {
        last_animation = new CinematicCamera(animation_queue[id].start, animation_queue[id].end, animation_queue[id].animation_time, animation_queue[id].cur_offset, false, animation_queue[id].beg_func, animation_queue[id].end_func);
        gameObject->AddScript(last_animation);
        ++id;
    }

    if (Input::getKeyClicked(GLFW_KEY_SPACE))
        skip();
}

void AnimationManager::Update() {}

void AnimationManager::OnDestroy() {}

void AnimationManager::addToQueue(data animation) {
    animation_queue.push_back(animation);
}

void AnimationManager::skip() {
    if (disabled_skips)
        return;
    if (last_animation == nullptr)
        return;
    last_animation->skip();
    while (id < animation_queue.size() && animation_queue[id].stop_skip == false)
        ++id;
}