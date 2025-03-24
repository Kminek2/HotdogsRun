﻿#include "BotMovement.h"

BotMovement::BotMovement(CarMovement* car)
{
    carmv = car;
}

void BotMovement::Init()
{
    botObject = gameObject;
    std::cout << "BotMovement::Init() called\n"; //debug
    this->botBehavior = AGGRESSIVE;
    chooseAction();
    currentWaypointIndex = 1;
}

void BotMovement::Update() {
    followPath();
    avoidObstacles();
}

void BotMovement::OnDestroy() {
    // TODO
}

BotMovement* BotMovement::SetMapManager(MapManager* map)
{
	this->map = map;
	return this;
}

std::vector<GameObject*> BotMovement::GetWaypoints(MapManager* map)
{
    if (!map) {
        std::cout << "MapManager is NULL!\n";
        return waypoints;
    }
    std::cout << "Get waypoints!\n";
    waypoints = *(map->GetPoints());
    return waypoints;
}

CarMovement* BotMovement::SetCarMovement(CarMovement* car)
{
    this->carmv = car;
    return car;
}

void BotMovement::chooseAction() {
    assert(botBehavior != BotBehavior::undefined);

    if (botBehavior == AGGRESSIVE) {
        botActions.accelerate = true;
        botActions.brake = true;
        botActions.turnLeft = true;
        botActions.turnRight = true;
        botActions.useNitro = true;
    }
    else if (botBehavior == DUMMY) {
        botActions.accelerate = true;
        botActions.brake = false;
        botActions.turnLeft = true;
        botActions.turnRight = true;
        botActions.useNitro = false;
    }
}

void BotMovement::followPath() {
    if (this->waypoints.empty()) {
        std::cout << "No waypoints! Bot can't move!\n";
        return;
    }
    if (this->carmv == nullptr) {
        std::cout << "Car movement not set! \n";
        return;
    }

    GameObject* targetWaypoint = waypoints[currentWaypointIndex];
    glm::vec3 targetPos = targetWaypoint->transform->position;
    glm::vec3 botPos = botObject->transform->position;
    float speed = carmv->getActSpeed();

    glm::vec3 rawDirection = targetPos - botPos;
    if (glm::dot(rawDirection, rawDirection) < 1e-6) return;
    glm::vec3 direction = glm::normalize(rawDirection);

    direction.z = 0;
    glm::vec3 front = glm::normalize(botObject->transform->front);

    float dotProduct = glm::dot(front, direction);
    float angle = glm::degrees(glm::acos(dotProduct));

    glm::vec3 crossProduct = glm::cross(front, direction);
    bool turnLeft = crossProduct.z > 0.1f;  
    bool turnRight = crossProduct.z < -0.1f; 

    float distance = glm::distance(botPos, targetPos);
    bool shouldBrake = false;

    if ((crossProduct.z > 0.5f || crossProduct.z < -0.5f) && speed > 50.0f) {
        shouldBrake = true;
    }

    bool shouldAccelerate = !shouldBrake;

    if (shouldAccelerate) {
        this->carmv->goForward();
    }
    else if (shouldBrake) {
        this->carmv->useHandBreak();
    }
    if (turnLeft) {
        this->carmv->makeLeftTurn();
    }
    else if (turnRight) {
        this->carmv->makeRightTurn();
    }

    if (distance < 8.0f) {
        currentWaypointIndex++;
        if (currentWaypointIndex >= waypoints.size()) {
            currentWaypointIndex = 0;
        }
    }
}

void BotMovement::avoidObstacles() {
    if (!map) return;

    glm::vec3 botPosition = botObject->transform->position;
    glm::vec3 frontDirection = glm::normalize(botObject->transform->front);
    float detectionRange = 1.5f; 

    bool obstacleDetected = false;
    glm::vec3 obstaclePos;
    

    for (GameObject* decor : map->getDecors()) {
        if (!decor) continue;

        glm::vec3 decorPosition = decor->transform->position;
        float distance = glm::distance(botPosition, decorPosition);

         if (distance < detectionRange) {
            glm::vec3 toObstacle = glm::normalize(decorPosition - botPosition);
            float dotProduct = glm::dot(frontDirection, toObstacle);

            if (dotProduct < 0.1f || dotProduct > -0.1f) { // check if obstacle is in front
                obstacleDetected = true;
                obstaclePos = decorPosition;
                break;
            }
        }
    }

    if (obstacleDetected) {
        botActions.accelerate = false;

        // avoid the obstacle
        glm::vec3 rightDirection = glm::cross(frontDirection, glm::vec3(0, 1, 0));
        glm::vec3 leftDirection = -rightDirection;

        glm::vec3 rightCheck = botPosition + rightDirection * 2.0f;
        glm::vec3 leftCheck = botPosition + leftDirection * 2.0f;

        float rightDist = glm::distance(rightCheck, obstaclePos);
        float leftDist = glm::distance(leftCheck, obstaclePos);

        if (leftDist > rightDist) {
            botActions.turnLeft = true;
            botActions.turnRight = false;
        }
        else {
            botActions.turnLeft = false;
            botActions.turnRight = true;
        }
    }
}