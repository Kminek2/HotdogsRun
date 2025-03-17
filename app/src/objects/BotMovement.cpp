#include "BotMovement.h"

void BotMovement::Init(MapManager* map)
{
	SetMapManager(map);
}

void BotMovement::Update() {
    chooseAction();         // Decyzje o tym, co bot ma zrobi�
    followPath();           // Oblicz, jak bot ma pod��a� za �cie�k�
    avoidObstacles();       // Sprawdzanie przeszk�d na drodze
    reactToOpponent();      // Reakcje na przeciwnik�w
}

void BotMovement::OnDestroy() {

// TODO
}

BotMovement* BotMovement::SetMapManager(MapManager* map)
{
	this->map = map;
	return this;
}

void BotMovement::chooseAction() {

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
    if (currentWaypointIndex < waypoints.size()) {
        glm::vec3 targetWaypoint = waypoints[currentWaypointIndex];
        glm::vec3 direction = targetWaypoint - botObject->transform->position;
        float distance = glm::length(direction);
        if (distance < .5f) {
            currentWaypointIndex++;
        }
        else {
            direction = glm::normalize(direction);

            glm::vec3 frontDirection = botObject->transform->front; // Przyjmujemy, �e bot ma wektor 'forward', kt�ry wskazuje na jego aktualny kierunek
            float angleToTarget = glm::degrees(atan2(direction.x, direction.z) - atan2(frontDirection.x, frontDirection.z));
            
            if (angleToTarget > 180.0f) angleToTarget -= 360.0f;
            if (angleToTarget < -180.0f) angleToTarget += 360.0f;

            // Ustalmy jak bot powinien obraca� si� w stron� celu
            if (angleToTarget > 0) {
                // Skr�caj w lewo
                makeLeftTurn();
            }
            else {
                // Skr�caj w prawo
                makeRightTurn();
            }
            goForward();
        }
    }
}

void BotMovement::avoidObstacles() {

    // TODO
}

void BotMovement::reactToOpponent() {
    // TODO
}