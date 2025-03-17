#pragma once
#include "objects/CarMovement.h"
#include "Collisions.h"
#include "GameObject.h"
#include "objects/MapManager.h"

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
        DUMMY
    } botBehavior;

    std::vector<glm::vec3> waypoints;
    int currentWaypointIndex;
    GameObject* botObject;
    MapManager* map;


    void handleCollision(Collisions::CollisionData* collisionData);

public:
    void Init(MapManager* map);
    void Update() override;
    void OnDestroy() override;
    void chooseAction();
    void followPath();
    void avoidObstacles();
    void reactToOpponent();
    BotMovement* SetMapManager(MapManager* map);
};
