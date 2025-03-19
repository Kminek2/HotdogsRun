#pragma once
#include "objects/CarMovement.h"
#include "Collisions.h"
#include "GameObject.h"
#include "objects/MapManager.h"
#include "ObjectScript.h"

class BotMovement : public ObjectScript{
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
    std::vector<GameObject*> waypoints;
    GameObject* botObject;
    MapManager* map;
    CarMovement* carmv;

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
