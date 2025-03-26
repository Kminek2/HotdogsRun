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

    glm::vec3 baseDirection = glm::normalize(targetPos - botPos);
    baseDirection.z = 0.0f;

    avoidanceVector = glm::vec3(0.0f);
    const float lateralThreshold = 1.8f;

    // Obstacle avoidance logic remains the same
    for (GameObject* obstacle : map->getDecors()) {
        glm::vec3 toObstacle = obstacle->transform->position - botPos;
        float distance = glm::length(toObstacle);

        // Pomijaj przeszkody poza zasięgiem lub zbyt bliskie
        if (distance > maxAvoidanceDistance || distance < 1.0f) continue;

        // Oblicz rzut na kierunek ruchu (czy przeszkoda jest przed nami)
        float forwardProj = glm::dot(toObstacle, baseDirection);

        // Ignoruj przeszkody za botem
        if (forwardProj <= 0) continue;

        // Oblicz odchylenie boczne od ścieżki
        glm::vec3 proj = baseDirection * forwardProj;
        glm::vec3 lateral = toObstacle - proj;
        float lateralDist = glm::length(lateral);

        // Ignoruj przeszkody z boku ścieżki
        if (lateralDist > lateralThreshold) continue;

        // Oblicz kierunek odpychania
        glm::vec3 obstacleDir = glm::normalize(toObstacle);
        glm::vec3 cross = glm::cross(baseDirection, obstacleDir);

        // Wybierz kierunek odpychania w zależności od strony przeszkody
        glm::vec3 repelDir = cross.z > 0 ?
            glm::vec3(baseDirection.y, -baseDirection.x, 0.0f) :  // Odpychaj w lewo
            glm::vec3(-baseDirection.y, baseDirection.x, 0.0f);   // Odpychaj w prawo

        // Oblicz siłę odpychania (im bliżej tym silniej)
        float strength = (maxAvoidanceDistance - distance) / maxAvoidanceDistance;
        avoidanceVector += glm::normalize(repelDir) * strength;
    }

    avoidanceVector *= avoidanceWeight;

    glm::vec3 desiredDirection = glm::normalize(baseDirection + avoidanceVector);

    // Steering calculation remains the same
    glm::vec3 currentFront = glm::normalize(botObject->transform->front);
    glm::vec3 cross = glm::cross(currentFront, desiredDirection);

    if (cross.z > 0.15f) {
        carmv->makeLeftTurn();
    }
    else if (cross.z < -0.15f) {
        carmv->makeRightTurn();
    }

    float speedModifier = 1.0f - glm::clamp(glm::length(avoidanceVector) / 0.8f, 0.0f, 0.1f);
    float targetSpeed = 600.0f * speedModifier;

    // CORRECTED: Check current waypoint's model instead of next
    std::string currentModel = currentWaypoint->GetModelName();

    // CORRECTED: Fixed typo in model name and adjusted speed reduction
    if (currentModel == "zakretPolSkosLod" || currentModel == "zakretLod" || currentModel == "zakretSkosOstryLod") {
        targetSpeed *= 0.1f; // Reduce speed to 30% for sharp turns
    }

    // Additional check for upcoming turns by looking at the next segment
    int nextNextIndex = (currentWaypointIndex + 1) % waypoints.size();
    glm::vec3 nextSegmentDir = glm::normalize(waypoints[nextNextIndex]->transform->position - targetPos);
    float turnAngle = glm::degrees(glm::acos(glm::dot(baseDirection, nextSegmentDir)));

    // If upcoming turn is sharp, reduce speed preemptively
    if (turnAngle > 30.0f) {
        targetSpeed *= 0.2f; // Additional 40% reduction for approaching sharp turns
    }

    if (carmv->getActSpeed() < targetSpeed) {
        carmv->goForward();
    }
    else {
        carmv->useHandBreak();
    }

    int nextIndex = (currentWaypointIndex + 1) % waypoints.size();
    nextSegmentDir = glm::normalize(
        waypoints[nextIndex]->transform->position - targetPos
    );
    turnAngle = glm::degrees(glm::acos(
        glm::dot(baseDirection, nextSegmentDir)
    ));

    // Oblicz dynamiczny próg
    float currentSpeed = carmv->getActSpeed();
    float baseDistance = 5.0f;  // Zmniejszona baza dla lepszej responsywności
    float speedFactor = glm::mix(0.5f, 1.8f, currentSpeed / 450.0f);  // Mniejszy maksymalny współczynnik
    float turnFactor = 1.0f + glm::clamp(turnAngle / 120.0f, 0.0f, 1.5f);  // Limitowany współczynnik zakrętu
    float dynamicDistance = baseDistance * speedFactor * turnFactor;

    // Antycypacja waypointów z zabezpieczeniem
    if (glm::distance(botPos, targetPos) < 23.0f) {
        currentWaypointIndex = (currentWaypointIndex + 1) % waypoints.size();
    }
}