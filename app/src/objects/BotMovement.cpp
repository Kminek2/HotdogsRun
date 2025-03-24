#include "BotMovement.h"

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
    bool shouldAccelerate = true;

    // W przypadku zakrętu, jeśli prędkość jest zbyt duża, zwolnij
    if ((turnLeft || turnRight) && speed > 70.0f) {
        shouldBrake = true;
        shouldAccelerate = false;
    }

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

    // Jeśli jesteśmy blisko waypointa, przechodzimy do następnego
    if (distance < 20.0f) {
        currentWaypointIndex++;
        if (currentWaypointIndex >= waypoints.size()) {
            currentWaypointIndex = 0;
        }
    }
}

void BotMovement::avoidObstacles() {
    if (this->carmv == nullptr || this->map == nullptr) {
        std::cout << "Car movement or map not set!\n";
        return;
    }

    glm::vec3 botPos = botObject->transform->position;
    glm::vec3 front = glm::normalize(botObject->transform->front);
    float detectionRange = 80.0f;  // Zwiększamy zasięg wykrywania przeszkód
    float avoidanceStrength = 0.5f; // Zwiększamy siłę korekty
    static float lastAvoidanceTime = 0.0f;  // Czas ostatniej reakcji na przeszkodę
    float cooldownTime = 1.0f;  // Minimalny czas między kolejnymi reakcjami na przeszkodę

    std::vector<GameObject*> obstacles = map->getDecors();
    glm::vec3 avoidanceVector(0.0f);
    bool obstacleAhead = false;

    // Iterujemy przez przeszkody
    for (GameObject* obstacle : obstacles) {
        glm::vec3 obstaclePos = obstacle->transform->position;
        glm::vec3 toObstacle = obstaclePos - botPos;

        float distance = glm::length(toObstacle);
        if (distance > detectionRange) continue; // Za daleko, pomijamy

        glm::vec3 toObstacleNorm = glm::normalize(toObstacle);
        float dotProduct = glm::dot(toObstacleNorm, front);

        if (dotProduct < 0.7f) continue;  // Teraz sprawdzamy tylko obiekty prawie na wprost

        obstacleAhead = true;

        glm::vec3 crossProduct = glm::cross(front, toObstacleNorm);
        // Ustalanie kierunku unikania
        if (crossProduct.z > 0) {
            avoidanceVector += glm::vec3(-front.y, front.x, 0) * avoidanceStrength;
        }
        else {
            avoidanceVector += glm::vec3(front.y, -front.x, 0) * avoidanceStrength;
        }

        // Jeśli przeszkoda jest bardzo blisko i znajduje się dokładnie na kursie, zwolnij
        if (distance < 20.0f && carmv->getActSpeed() > 30.0f) {
            std::cout << "Obstacle too close! Braking!\n";
            this->carmv->useHandBreak();
        }
    }

    // Jeśli przeszkody są zbyt blisko i jesteśmy w trakcie "uniku", dajemy im chwilę na "chłodzenie"
    if (obstacleAhead && glm::length(avoidanceVector) > 0.01f) {
        float currentTime = static_cast<float>(glfwGetTime());
        if (currentTime - lastAvoidanceTime > cooldownTime) {
            glm::vec3 newDirection = glm::normalize(front + avoidanceVector);
            float correctionAngle = glm::degrees(glm::acos(glm::dot(front, newDirection)));

            if (correctionAngle > 5.0f) { // Unikamy niepotrzebnych korekt
                if (avoidanceVector.x > 0) {
                    this->carmv->makeRightTurn();
                }
                else {
                    this->carmv->makeLeftTurn();
                }
            }

            lastAvoidanceTime = currentTime; // Zaktualizuj czas ostatniego uniku
        }
    }
}