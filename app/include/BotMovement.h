#pragma once

#include "objects/CarMovement.h"
#include "objects/MapManager.h"

#include "ObjectScript.h"
#include "Collisions.h"
#include "GameObject.h"

class BotMovement : public ObjectScript {
private:
    struct bot_actions {
        bool accelerate = false;
        bool brake = false;
        bool turnLeft = false;
        bool turnRight = false;
        bool useNitro = false;
    } botActions;

    enum BotBehavior {
        undefined,
        AGGRESSIVE,
        DUMMY
    } botBehavior = undefined;

    int currentWaypointIndex = 0;
    std::vector<GameObject*> waypoints;
    GameObject* botObject = nullptr;
    MapManager* map = nullptr;
    CarMovement* carmv = nullptr;

public:
    BotMovement(CarMovement* car);

    void Init() override;
    void Update() override;
    void OnDestroy() override;

    void chooseAction();
    void followPath();
    void avoidObstacles();

    BotMovement* SetMapManager(MapManager* map);
    std::vector<GameObject*> GetWaypoints(MapManager* map);
    CarMovement* SetCarMovement(CarMovement* car);
};
