#pragma once
#include "CarMovement.h"
#include "Collisions.h"
#include "GameObject.h"

class BotMovement : public CarMovement {
private:
    struct bot_actions {
        bool accelerate;
        bool brake;
        bool turnLeft;
        bool turnRight;
        bool useNitro;
    } botActions;

    enum BotBehavior {
        AGGRESSIVE,
        DEFENSIVE,
        TIMID,
        RECKLESS
    } botBehavior;

    std::vector<glm::vec3> waypoints = ;
    int currentWaypointIndex;
    GameObject* botObject;


    void handleCollision(Collisions::CollisionData* collisionData);

public:
    BotMovement(GameObject* bot, float carWeight, float breaksStrength, float maxSpeed, float minSpeed, float accelFront, float accelBack, float gripToSpeedMult, bool expertMode = false, float multiplier = 0.1f, glm::vec3 nitro_trail_offset = glm::vec3(1.7f, 0.0f, 0.0f));

    void Init(Map) override;
    void Update() override;
    void OnDestroy() override;
    void chooseAction();
    void followPath();
    void avoidObstacles();
    void reactToOpponent();
    void applyBotActions();
    void updateSurfaceEffect();
};
