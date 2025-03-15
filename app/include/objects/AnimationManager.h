#pragma once
#include "ObjectScript.h"
#include "objects/CinematicCamera.h"
#include <vector>

class AnimationManager :
    public ObjectScript {
    public:
        struct data {
            CinematicCamera::obj_data start;
            CinematicCamera::obj_data end;
            float animation_time;
            glm::vec3 cur_offset;
            bool stop_skip = false;
            std::function<void()> beg_func = [](){};
            std::function<void()> end_func = [](){};
        };
        AnimationManager(std::vector<data> animation_queue = {});
        void Init() override;
        void Update() override;
        void OnDestroy() override;
        void addToQueue(data animation);
        void skip();
    private:
        int id;
        CinematicCamera* last_animation;
        std::vector<data> animation_queue;
};

