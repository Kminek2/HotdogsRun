#include "BotMovement.h"

BotMovement::BotMovement(GameObject* bot, float carWeight, float breaksStrength, float maxSpeed, float minSpeed, float accelFront, float accelBack, float gripToSpeedMult, bool expertMode, float multiplier, glm::vec3 nitro_trail_offset)
    : CarMovement(carWeight, breaksStrength, maxSpeed, minSpeed, accelFront, accelBack, gripToSpeedMult, expertMode, multiplier, nitro_trail_offset)
{
    botObject = bot;
    currentWaypointIndex = 0;
}

void BotMovement::Init()
{
    std::cout << "BotMovement::Init() called\n"; //debug
    this->botBehavior = AGGRESSIVE;
    currentWaypointIndex = 0;
}

void BotMovement::Update() {
    followPath();
    chooseAction();
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

void BotMovement::getWaypoints(MapManager* map)
{
    if (!map) {
        std::cout << "❌ MapManager is NULL!\n";
        return;
    }
    std::cout << "Get waypoints!\n";
    waypoints = *(map->GetPoints());
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
    if (this->waypoints.empty()) {
        std::cout << "No waypoints! Bot can't move!\n";
        return;
    }

    // current waypoint
    GameObject* targetWaypoint = waypoints[currentWaypointIndex];
    glm::vec3 targetPos = targetWaypoint->transform->position;

    glm::vec3 botPos = botObject->transform->position;

    // direction vector
    glm::vec3 direction = glm::normalize(targetPos - botPos);
    float speed = 15.0f; //debug

    botObject->transform->Move(direction * speed * Time::deltaTime);

    // check if reached waypoint
    float distance = glm::distance(botPos, targetPos);
    if (distance < 5.0f) { // if close, go to the next one
        currentWaypointIndex++;
        if (currentWaypointIndex >= waypoints.size()) currentWaypointIndex = 0; // start the next lap
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
