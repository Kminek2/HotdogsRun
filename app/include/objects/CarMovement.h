#pragma once

#include "ObjectScript.h"
#include "GameObject.h"
#include "Collisions.h"

#include "objects/LockPosition.h"
#include "objects/LockRotation.h"
#include "AudioSource2d.h"

constexpr float EPSILON = 1e-3;

class WheelsScript;

class CarMovement :
    public ObjectScript {
private:
    // default value        used
    const float multiplier; float __multiplier;
    const float maxSpeed;   float __maxSpeed;
    const float minSpeed;
    const float accelFront; float __accelFront;
    const float accelBack;
    const float carWeight;  float __carWeight; // (SCALE UNIT) NORMAL BREAKS - 1
    const float breaksStrength; // IN TONES
    const float gripToSpeed;
    const bool expertMode;
    float actSpeed;
    float axleAngle;
    glm::vec3 gripMult;
    glm::vec3 forces;
    float downSpeed = 0;
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

    float crashsound_timer;
    float gassound_timer;

    AudioSource2d* crashsound_audio;
    AudioSource2d* gassound_audio;
public:
    CarMovement(float carWeight, float breaksStrength, float maxSpeed, float minSpeed, float accelFront, float accelBack, float gripToSpeedMult, bool expertMode = false, float multiplier = 0.1f, glm::vec3 nitro_trail_offset = glm::vec3(1.7f, 0.0f, 0.0f));
    void Init() override;
    void Update() override;
    void OnDestroy() override;
    void goForward();
    void goBackwards();
    void makeLeftTurn();
    void makeRightTurn();
    void useHandBreak();
    void useNitro();
    void handleAudio();
    float getActSpeed();
    float getAxleAngle();
    friend WheelsScript;
    
    // overrides [percent change]
    float _multiplier = 1;
    float _maxSpeed   = 1;
    float _accelFront = 1;
    float _carWeight  = 1;
    
    static bool disabled_inputs;

    void addNitros(int count);
};