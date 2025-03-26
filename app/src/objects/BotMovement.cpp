#include "BotMovement.h"
#include <glm/gtx/norm.hpp>
#include <iostream>

BotMovement::BotMovement(CarMovement* car)
    : avoidanceVector(0.0f),
    avoidanceWeight(2.0f),
    maxAvoidanceDistance(120.0f),
    currentWaypointIndex(0),
    botObject(nullptr),
    map(nullptr),
    carmv(car) {}

void BotMovement::Init() {
    botObject = gameObject;
    std::cout << "BotMovement::Init() called\n";
    this->botBehavior = AGGRESSIVE;
    chooseAction();
    waypoints = GetWaypoints(map);
    currentWaypointIndex = 1;
}

void BotMovement::Update() {
    if (!carmv || !map) return;

    followPath();
}

void BotMovement::OnDestroy() {
    // Cleanup logic if needed
}

BotMovement* BotMovement::SetMapManager(MapManager* map) {
    this->map = map;
    return this;
}

std::vector<GameObject*> BotMovement::GetWaypoints(MapManager* map) {
    if (!map) {
        std::cout << "MapManager is NULL!\n";
        return {};
    }
    return *(map->GetPoints());
}

CarMovement* BotMovement::SetCarMovement(CarMovement* car) {
    this->carmv = car;
    return car;
}

void BotMovement::chooseAction() {
    assert(botBehavior != BotBehavior::undefined);

    if (botBehavior == AGGRESSIVE) {
        botActions = { true, true, true, true, true };
    }
    else if (botBehavior == DUMMY) {
        botActions = { true, false, true, true, false };
    }
}

glm::vec3 BotMovement::calculateDesiredDirection() const {
    if (waypoints.empty()) return glm::vec3(0);

    glm::vec3 baseDirection = glm::normalize(
        waypoints[currentWaypointIndex]->transform->position -
        botObject->transform->position
    );
    return glm::normalize(baseDirection + avoidanceVector * avoidanceWeight);
}

float BotMovement::calculateSpeedModifier() const {
    const float currentSpeed = carmv->getActSpeed();
    const glm::vec3 desiredDir = calculateDesiredDirection();
    const float angle = glm::degrees(glm::acos(
        glm::dot(glm::normalize(botObject->transform->front), desiredDir)
    ));

    float speedModifier = 1.0f - glm::clamp(angle / 45.0f, 0.0f, 0.8f);

    if (glm::length(avoidanceVector) > 0.3f) {
        speedModifier *= 0.6f;
    }

    return speedModifier;
}

void BotMovement::followPath() {
    if (waypoints.empty() || !carmv || !map) return;

    GameObject* currentWaypoint = waypoints[currentWaypointIndex];
    glm::vec3 botPos = botObject->transform->position;
    glm::vec3 targetPos = currentWaypoint->transform->position;

    // Oblicz bazowy kierunek do waypointa
    glm::vec3 baseDirection = glm::normalize(targetPos - botPos);
    baseDirection.z = 0.0f;

    // Inicjalizacja wektora unikania
    avoidanceVector = glm::vec3(0.0f);
    const float lateralThreshold = 2.0f;

    // Analizuj wszystkie przeszkody
    for (GameObject* obstacle : map->getDecors()) {
        glm::vec3 toObstacle = obstacle->transform->position - botPos;
        float distance = glm::length(toObstacle);

        if (distance > maxAvoidanceDistance || distance < 1.0f) continue;

        // Sprawdź czy przeszkoda jest przed botem
        float forwardProj = glm::dot(toObstacle, baseDirection);
        if (forwardProj <= 0) continue;

        // Oblicz odległość boczną
        glm::vec3 proj = baseDirection * forwardProj;
        glm::vec3 lateral = toObstacle - proj;
        float lateralDist = glm::length(lateral);
        if (lateralDist > lateralThreshold) continue;

        // Określ kierunek omijania
        glm::vec3 obstacleDir = glm::normalize(toObstacle);
        glm::vec3 cross = glm::cross(baseDirection, obstacleDir);
        glm::vec3 repelDir = cross.z > 0 ?
            glm::vec3(baseDirection.y, -baseDirection.x, 0.0f) : // W prawo
            glm::vec3(-baseDirection.y, baseDirection.x, 0.0f);  // W lewo

        // Siła unikania zależna od odległości
        float strength = (maxAvoidanceDistance - distance) / maxAvoidanceDistance;
        avoidanceVector += glm::normalize(repelDir) * strength;
    }

    // Zastosuj globalną wagę unikania
    avoidanceVector *= avoidanceWeight;

    // Oblicz końcowy kierunek ruchu
    glm::vec3 desiredDirection = glm::normalize(baseDirection + avoidanceVector);

    // Sterowanie
    glm::vec3 currentFront = glm::normalize(botObject->transform->front);
    glm::vec3 cross = glm::cross(currentFront, desiredDirection);

    if (cross.z > 0.1f) {
        carmv->makeLeftTurn();
    }
    else if (cross.z < -0.1f) {
        carmv->makeRightTurn();
    }

    // Kontrola prędkości
    float speedModifier = 1.0f - glm::clamp(glm::length(avoidanceVector) / 2.0f, 0.0f, 0.7f);
    float targetSpeed = 600.0f * speedModifier;

    if (carmv->getActSpeed() < targetSpeed) {
        carmv->goForward();
    }
    else {
        carmv->useHandBreak();
    }

    // Aktualizacja waypointa
    if (glm::distance(botPos, targetPos) < 25.0f) {
        currentWaypointIndex = (currentWaypointIndex + 1) % waypoints.size();
    }
}