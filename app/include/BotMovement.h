#pragma once

#include "objects/CarMovement.h"
#include "objects/MapManager.h"
#include "ObjectScript.h"
#include "Collisions.h"
#include "GameObject.h"
#include <glm/glm.hpp>

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

    // Sterowanie
    glm::vec3 avoidanceVector;
    float avoidanceWeight = 1.5f;
    float maxAvoidanceDistance = 100.0f;

    // Œcie¿ka
    int currentWaypointIndex = 0;
    std::vector<GameObject*> waypoints;

    // Referencje
    GameObject* botObject = nullptr;
    MapManager* map = nullptr;
    CarMovement* carmv = nullptr;

public:
    BotMovement(CarMovement* car);

    void Init() override;
    void Update() override;
    void OnDestroy() override;

    // Nowe metody pomocnicze
    glm::vec3 calculateDesiredDirection() const;
    float calculateSpeedModifier() const;

    // G³ówne funkcje steruj¹ce
    void chooseAction();
    void followPath();
    void avoidObstacles();

    // Setters
    BotMovement* SetMapManager(MapManager* map);
    CarMovement* SetCarMovement(CarMovement* car);

    // Gettery
    std::vector<GameObject*> GetWaypoints(MapManager* map);
};