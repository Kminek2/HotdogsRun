#pragma once
#include "ObjectScript.h"
#include "GameObject.h"
#include "Collisions.h"

constexpr float EPSILON = 1e-3;

class CarMovement :
    public ObjectScript {
private:
    const float multiplier;
    const float maxSpeed;
    const float minSpeed;
    const float accelFront;
    const float accelBack;
    const float carWeight; // (SCALE UNIT) NORMAL BREAKS - 1
    const float breaksStrength; // IN TONES
    const bool expertMode;
    float actSpeed;
    float axleAngle;
    glm::vec3 forces;
    void handleGas();
    void handleBreaks();
    void handleSteeringWheel();
    void handleEngBreak();
    void handleForces();
public:
    CarMovement(float carWeight, float breaksStrength, float maxSpeed, float minSpeed, float accelFront, float accelBack, bool expertMode = false, float multiplier = 0.1f);
    void Init() override;
    void Update() override;
    void OnDestroy() override;
};