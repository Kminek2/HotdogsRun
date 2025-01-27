#pragma once
#include "ObjectScript.h"
#include "CameraStructures.h"
#include "GameObject.h"
class CameraLockScript :
    public ObjectScript {
    private:
        glm::vec3 offset = glm::vec3(0.0f);
        float pitch = 0.0f;
        float yaw = 0.0f;
        ViewType view = Isometric;
    public:
        CameraLockScript(ViewType view = Isometric, glm::vec3 offset = glm::vec3(-300.0f, -300.0f, 150.0f), float pitch = -15.0f, float yaw = 45.0f);
        void Init() override;
        void Update() override;
        void OnDestroy() override;
};

