#include "BotMovement.h"

BotMovement::BotMovement(CarMovement* car)
{
    currentWaypointIndex = 0;
    carmv = car;
}

void BotMovement::Init()
{
    botObject = gameObject;
    std::cout << "BotMovement::Init() called\n"; //debug
    this->botBehavior = AGGRESSIVE;
    chooseAction();
    currentWaypointIndex = 0;
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

    // Kierunek do waypointa - sprawdzenie przed normalizacją
    glm::vec3 rawDirection = targetPos - botPos;
    if (glm::dot(rawDirection, rawDirection) < 0.0001f) return; // Używamy kwadratu długości do porównania
    glm::vec3 direction = glm::normalize(rawDirection);

    direction.z = 0;
    // Wektor front bota
    glm::vec3 front = glm::normalize(botObject->transform->front);

    // Kąt między botem a waypointem
    float dotProduct = glm::dot(front, direction);
    std::cout << "dotProduct: " << dotProduct << "\n";

    // Obliczanie kąta między botem a waypointem (jeśli chcesz kąt w radianach)
    float angle = glm::degrees(glm::acos(dotProduct));
    std::cout << "Angle (degrees): " << angle << "\n";

    // Wykrywanie skrętu - Używamy crossProduct.Z zamiast crossProduct.Y!!!
    glm::vec3 crossProduct = glm::cross(front, direction);
    std::cout << "CrossProduct.z: " << crossProduct.z << "\n";
    bool turnLeft = crossProduct.z > .1f;  // Jeśli Z jest dodatnie -> skręt w lewo
    bool turnRight = crossProduct.z < -.1f; // Jeśli Z jest ujemne -> skręt w prawo

    // Logika hamowania i przyspieszania
    float distance = glm::distance(botPos, targetPos);
    bool shouldBrake = false;

    if (distance < 2.0f && (crossProduct.z >.1f || crossProduct.z <-.1f)) {
        shouldBrake = true;
    }

    bool shouldAccelerate = !shouldBrake;

    // Debugowanie logiki sterowania
    std::cout << "Should Brake: " << shouldBrake << "\n";
    std::cout << "Should Accelerate: " << shouldAccelerate << "\n";

    // Wysyłanie poleceń do samochodu
    if (shouldAccelerate) {
        std::cout << "Accelerating!\n";
        this->carmv->goForward();
    }
    else if (shouldBrake) {
        std::cout << "Braking!\n";
        this->carmv->useHandBreak();
    }
    if (turnLeft) {
        std::cout << "Turning Left!\n";
        this->carmv->makeLeftTurn();
    }
    else if (turnRight) {
        std::cout << "Turning Right!\n";
        this->carmv->makeRightTurn();
    }

    // Przejście do następnego waypointa
    if (distance < 5.0f) {
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
    float detectionRange = 5.0f; 

    bool obstacleDetected = false;
    glm::vec3 obstaclePos;

    for (GameObject* decor : map->getDecors()) {
        if (!decor) continue;

        glm::vec3 decorPosition = decor->transform->position;
        float distance = glm::distance(botPosition, decorPosition);

         if (distance < detectionRange) {
            glm::vec3 toObstacle = glm::normalize(decorPosition - botPosition);
            float dotProduct = glm::dot(frontDirection, toObstacle);

            if (dotProduct > 0.7f) { // check if obstacle is in front
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