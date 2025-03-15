#pragma once
#include "ObjectScript.h"
#include "GameObject.h"
#include "Collisions.h"
#include "objects/LockPosition.h"
#include "objects/LockRotation.h"

constexpr float EPSILON = 1e-3;

class WheelsScript;

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
    glm::vec3 gripMult;
    glm::vec3 forces;
    const glm::vec3 nitro_trail_offset;
    void handleGas();
    void handleBreaks();
    void handleSteeringWheel();
    void handleEngBreak();
    void handleForces();
    void handleNitroAcc();
    void handleGrip();
    struct actions {
        bool forward;
        bool backwards;
        bool left_turn;
        bool right_turn;
        bool hand_break;
    } actActions;
    struct road_type_data {
        road_type_data(float acc_multiplier, float eng_break_multiplier, float break_multiplier, float steering_multiplier, float max_speed_multiplier, float grip);
        float acc_multiplier;
        float eng_break_multiplier;
        float break_multiplier;
        float steering_multiplier;
        float max_speed_multiplier;
        float grip;
    };
    float nitro_timer;
    float before_nitro_mem;
    int nitros_available;
    GameObject* nitro_trail;
    static const actions clearedActions;
    static const std::array<road_type_data,5> surfaces_data;
    static const float nitro_duration;
    int road_type; // 0 - no surface (grass), 1 - classical road (asphalt), 2 - dirt road, 3 - icy road, 4 - oil puddle
public:
    CarMovement(float carWeight, float breaksStrength, float maxSpeed, float minSpeed, float accelFront, float accelBack, bool expertMode = false, float multiplier = 0.1f, glm::vec3 nitro_trail_offset = glm::vec3(1.7f, 0.0f, 0.0f));
    void Init() override;
    void Update() override;
    void OnDestroy() override;
    void goForward();
    void goBackwards();
    void makeLeftTurn();
    void makeRightTurn();
    void useHandBreak();
    void useNitro();
    friend WheelsScript;
};