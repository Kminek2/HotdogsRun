#include "objects/StraightKingBot.h"
#include "objects/ShowOBB.h"

StraightKingBot::StraightKingBot(CarMovement* carMovement, MapManager* map, float brakMult) : carMovement(carMovement), map(map), points(*map->GetPoints()), breakMult(brakMult)
{
	currentPoint = 1;
	toPoint = glm::vec3(0);
}

void StraightKingBot::Init()
{
	carSize = gameObject->GetModelMaxDistVert()[0].x - gameObject->GetModelMaxDistVert()[0].y;
	glm::vec3 obbSizes = glm::vec3(carSize * 2, gameObject->GetModelMaxDistVert()[1].x - gameObject->GetModelMaxDistVert()[1].y, gameObject->GetModelMaxDistVert()[2].x - gameObject->GetModelMaxDistVert()[2].y);

	antiCollider = new GameObject("", gameObject->transform->position, gameObject->transform->rotation, gameObject->transform->scale, NEVER_COLLIDE);
	antiCollider->addOBB(*new OBB({ -carSize * 2 / 2, 0, 0 }, obbSizes / 2.0f));
}

void StraightKingBot::EarlyUpdate()
{
	if (HandlePredictions()) {
		if (carMovement->getActSpeed() / carMovement->getMaxSpeed() < breakMult)
			carMovement->goForward();

		return;
	}

	float nextPointDot = glm::dot(glm::normalize(glm::vec2(gameObject->transform->front)), toPoint);

	glm::vec2 nextToPoint = toPoint;
	if (!MovedOverPoint(gameObject->transform->position))
		nextToPoint = glm::normalize(glm::vec2(points[currentPoint]->transform->position - gameObject->transform->position));

	float thisPointDot = glm::dot(glm::normalize(glm::vec2(gameObject->transform->front)), toPoint);

	if (abs(1 - nextPointDot) > breakMult && abs(1 - thisPointDot) > breakMult && carMovement->getActSpeed() / carMovement->getMaxSpeed() > breakMult)
		carMovement->useHandBreak();
	else {
		carMovement->goForward();
		float nextPointDot2 = glm::dot(glm::normalize(glm::vec2(gameObject->transform->front)), glm::normalize(glm::vec2(points[(currentPoint + 2) % points.size()]->transform->position - gameObject->transform->position)));
		float nextPointDot3 = glm::dot(glm::normalize(glm::vec2(gameObject->transform->front)), glm::normalize(glm::vec2(points[(currentPoint + 3) % points.size()]->transform->position - gameObject->transform->position)));
		if (abs(1 - nextPointDot) < breakMult && abs(1 - thisPointDot) < breakMult && abs(1 - nextPointDot2) < breakMult / 3.0f && abs(1 - nextPointDot3) < breakMult / 4.0f)
			carMovement->useNitro();
	}

	toPoint = nextToPoint;

	nextPointDot = glm::dot(glm::normalize(glm::vec2(gameObject->transform->right)), toPoint);
	float forwardDot = glm::dot(glm::normalize(glm::vec2(gameObject->transform->right)), toPoint);

	if (nextPointDot < -breakMult / 2.0f)
		carMovement->makeLeftTurn();
	else if (nextPointDot > breakMult / 2.0f)
		carMovement->makeRightTurn();
	else if (forwardDot < 0) {
		if(nextPointDot <= 0)
			carMovement->makeLeftTurn();
		else
			carMovement->makeRightTurn();
	}
}

void StraightKingBot::Update()
{
}

void StraightKingBot::LateUpdate()
{
	antiCollider->transform->MoveTo(gameObject->transform->position);
	antiCollider->transform->RotateTo(gameObject->transform->rotation);
	antiCollider->transform->ScaleTo(gameObject->transform->scale);
}

void StraightKingBot::OnDestroy()
{
	delete antiCollider;
}

bool StraightKingBot::MovedOverPoint(glm::vec3 pos)
{
	if (glm::dot(glm::normalize(glm::vec2(gameObject->transform->front)), toPoint) < 0 && glm::distance(points[currentPoint]->transform->position, pos) < 20)
		return true;

	return false;
}

bool StraightKingBot::HandlePredictions()
{
	glm::vec3 futurePos = carMovement->getFuturePos() * (1 / Time::deltaTime);
	gameObject->transform->Move(futurePos);
	if (MovedOverPoint(gameObject->transform->position))
		currentPoint = (currentPoint + 1) % points.size();
	
	toPoint = glm::normalize(glm::vec2(points[currentPoint]->transform->position - gameObject->transform->position));

	gameObject->transform->Move(-futurePos);

	bool colli = HandleCollision();

	return colli;
}

bool StraightKingBot::HandleCollision()
{
	GameObject* coll = nullptr;

	for (GameObject* obj : GameObject::getAllGameObjects()) {
		if (obj->surface_type == ALWAYS_COLLIDE && obj != gameObject && Collisions::checkCollision(*obj, *antiCollider)) {
			coll = obj;
			break;
		}
	}

	if (coll == nullptr)
		return false;

	glm::vec2 toColl = glm::normalize(glm::vec2(coll->transform->position - gameObject->transform->position));
	float nextPointDot = glm::dot(glm::normalize(glm::vec2(gameObject->transform->right)), toColl);

	if (nextPointDot > 0)
		carMovement->makeLeftTurn();
	else if (nextPointDot < 0)
		carMovement->makeRightTurn();

	return true;
}
