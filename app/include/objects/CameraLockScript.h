#pragma once

#include "ObjectScript.h"
#include "CameraStructures.h"
#include "GameObject.h"

class CinematicCamera;

class CameraLockScript :
    public ObjectScript {
    private:
        glm::vec3 offset = glm::vec3(0.0f);
        float pitch = 0.0f;
        float yaw = 0.0f;
        const ViewType view;
        const bool lock_rotation;
        const bool unlocked_rotation;
        const uint16_t key_left, key_right;
        int cnt_after_disabled = 0;
    public:
        CameraLockScript(ViewType view = Isometric, glm::vec3 offset = glm::vec3(-300.0f, 0.0f, 0.0f), float pitch = -15.0f, float yaw = 45.0f, bool lock_rotation = false, bool unlocked_rotation = false, uint16_t key_left = GLFW_KEY_Q, uint16_t key_right = GLFW_KEY_E);
        void Init() override;
        void Update() override;
        void LateUpdate() override;
        void OnDestroy() override;
        friend CinematicCamera;
        static bool disabled;
        bool disabled2;

        CameraLockScript* copy() override { return new CameraLockScript(*this); }
};

