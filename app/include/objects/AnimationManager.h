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
        };
        AnimationManager(std::vector<data> animation_queue = {});
        void Init() override;
        void Update() override;
        void OnDestroy() override;
        void addToQueue(data animation);
    private:
        int id;
        CinematicCamera* last_animation;
        std::vector<data> animation_queue;
};

