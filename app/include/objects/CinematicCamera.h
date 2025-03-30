#pragma once
#include "ObjectScript.h"
#include "CameraStructures.h"
#include "GameObject.h"
#include "objects/CameraLockScript.h"
#include <utility>

class CinematicCamera :
    public ObjectScript {
    public:
        struct obj_data {
            glm::vec3 position;
            glm::vec2 rotation;
        } old_data;
        ViewType old_view;
        CinematicCamera(obj_data begining, obj_data ending, float animation_time, glm::vec3 offset, bool ret_to_pos = true, std::function<void()> beg_func = [](){}, std::function<void()> end_func = [](){});
        void Init() override;
        void Update() override;
        void LateUpdate() override;
        void OnDestroy() override;
        bool hasEnded();
        void skip();
    private:
        obj_data position_in_time(float t);
        float time;
        const obj_data begining, ending;
        const glm::vec3 offset;
        const float animation_time;
        const bool ret_to_pos;
        const std::function<void()> beg_func;
        const std::function<void()> end_func;
        bool ended;

        CinematicCamera* copy() override { return new CinematicCamera(*this); }
};

