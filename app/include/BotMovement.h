#pragma once
#include "objects/CarMovement.h"
#include "Collisions.h"
#include "GameObject.h"
#include "objects/MapManager.h"
#include "ObjectScript.h"

class BotMovement : public CarMovement{
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
        DUMMY
    } botBehavior;

    int currentWaypointIndex;
    GameObject* botObject;
    MapManager* map;

    void handleCollision(Collisions::CollisionData* collisionData);

public:
    BotMovement(GameObject* bot, float carWeight, float breaksStrength, float maxSpeed, float minSpeed, float accelFront, float accelBack, float gripToSpeedMult, bool expertMode = false, float multiplier = 0.1f, glm::vec3 nitro_trail_offset = glm::vec3(1.7f, 0.0f, 0.0f));
    void Init() override;
    void Update() override;
    void OnDestroy() override;
    void chooseAction();
    void followPath();
    void avoidObstacles();
    BotMovement* SetMapManager(MapManager* map);
    void getWaypoints(MapManager* map);
    
    std::vector<GameObject*> waypoints; //debug

};
