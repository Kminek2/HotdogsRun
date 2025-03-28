#include "objects/MedBot.h"
#include "objects/ShowOBB.h"

MedBot::MedBot(CarMovement* carMovement, MapManager* map, RaceManager::CarObject* thisCarObj, float prefaredSpeed, float breakPower, float brakMult) : carMovement(carMovement), map(map), points(*map->GetPoints()), breakMult(brakMult), breakPower(breakPower), prefaredSpeed(prefaredSpeed), thisCar(thisCarObj)
{
	currentPoint = 1;
	toPoint = glm::vec3(0);
	avoiding = 0;
	changedPoint = false;
}

void MedBot::Init()
{
	carSize = gameObject->GetModelMaxDistVert()[0].x - gameObject->GetModelMaxDistVert()[0].y;
	glm::vec3 obbSizes = glm::vec3(carSize * 3, gameObject->GetModelMaxDistVert()[1].x - gameObject->GetModelMaxDistVert()[1].y, gameObject->GetModelMaxDistVert()[2].x - gameObject->GetModelMaxDistVert()[2].y) * glm::vec3(1.1f, 1.3f, 1.3f);

	antiCollider = new GameObject("", gameObject->transform->position, gameObject->transform->rotation, gameObject->transform->scale, NEVER_COLLIDE);
	antiCollider->addOBB(*new OBB({ -carSize * 3 / 2, 0, 0 }, obbSizes / 2.0f));
}

void MedBot::EarlyUpdate()
{
	bool changesToPoint = changedPoint;
	changedPoint = false;
	if (HandlePredictions()) {
		if (carMovement->getActSpeed() / carMovement->getMaxSpeed() < breakMult)
			carMovement->goForward();

		return;
	}

	avoiding = 0;

	float nextPointDot = glm::dot(glm::normalize(glm::vec2(gameObject->transform->front)), toPoint);
	glm::vec3 fromLastPoint = glm::normalize(points[glm::normalize((long long)currentPoint - 1, (long long)points.size())]->transform->position - points[glm::normalize((long long)currentPoint - 2, (long long)points.size())]->transform->position);

	if (changesToPoint || changedPoint) {
		glm::vec2 tmpToPoint = toPoint;
		toPoint = glm::normalize((glm::vec2((points[glm::normalize((long long)currentPoint - 1, (long long)points.size())]->transform->position + fromLastPoint) - (gameObject->transform->position))));
		if (!MovedOverPoint(gameObject->transform->position + gameObject->transform->front * 5.0f, 1))
			changedPoint = true;
		else
			changedPoint = false;
		toPoint = tmpToPoint;
	}

	glm::vec2 nextToPoint = toPoint;
	if (changedPoint && !HandleCheckPoint())
		nextToPoint = glm::normalize(glm::vec2((points[glm::normalize((long long)currentPoint - 1, (long long)points.size())]->transform->position + fromLastPoint / 2.0f) - gameObject->transform->position));
	else
		nextToPoint = toPoint;

	float thisPointDot = glm::dot(glm::normalize(glm::vec2(gameObject->transform->front)), glm::normalize(glm::vec2((points[glm::normalize((long long)currentPoint - 1, (long long)points.size())]->transform->position + fromLastPoint / 2.0f) - gameObject->transform->position)));

	if (abs(1 - nextPointDot) > breakMult && carMovement->getActSpeed() / carMovement->getMaxSpeed() > glm::max(((nextPointDot + 1) * 0.25f) / (breakPower * 4.0f), breakPower))
		carMovement->useHandBreak();
	else if (carMovement->getActSpeed() / carMovement->getMaxSpeed() < prefaredSpeed) {
		carMovement->goForward();
		float nextPointDot2 = glm::dot(glm::normalize(glm::vec2(gameObject->transform->front)), glm::normalize(glm::vec2(points[(currentPoint + 1) % points.size()]->transform->position - gameObject->transform->position)));
		float nextPointDot3 = glm::dot(glm::normalize(glm::vec2(gameObject->transform->front)), glm::normalize(glm::vec2(points[(currentPoint + 2) % points.size()]->transform->position - gameObject->transform->position)));
		if (abs(1 - nextPointDot) < breakMult && abs(1 - nextPointDot2) < breakMult / 300.0f && abs(1 - nextPointDot3) < breakMult / 300.0f && carMovement->getActSpeed() / carMovement->getMaxSpeed() < prefaredSpeed * (3.0f / 4.0f))
			carMovement->useNitro();
	}

	nextPointDot = glm::dot(glm::normalize(glm::vec2(gameObject->transform->right)), nextToPoint);
	float forwardDot = glm::dot(glm::normalize(glm::vec2(gameObject->transform->right)), nextToPoint);

	if (nextPointDot < -breakMult * glm::max(abs(carMovement->getAxleAngle() / 10.0f), 0.0001f)) {
		carMovement->makeLeftTurn();
		avoiding = 1;
	}
	else if (nextPointDot > breakMult * glm::max(abs(carMovement->getAxleAngle() / 10.0f), 0.0001f)) {
		carMovement->makeRightTurn();
		avoiding - 1;
	}
	else if (forwardDot < 0) {
		if (nextPointDot <= 0) {
			carMovement->makeLeftTurn();
			avoiding = 1;
		}
		else {
			carMovement->makeRightTurn();
			avoiding = -1;
		}
	}
	else
		avoiding = 0;
}

void MedBot::Update()
{
}

void MedBot::LateUpdate()
{
	antiCollider->transform->MoveTo(gameObject->transform->position);
	antiCollider->transform->RotateTo(gameObject->transform->rotation);
	antiCollider->transform->ScaleTo(gameObject->transform->scale);
}

void MedBot::OnDestroy()
{
	delete antiCollider;
}

bool MedBot::MovedOverPoint(glm::vec3 pos, int previous)
{
	long long pointToCheck = glm::normalize((long long)currentPoint - previous, (long long)points.size());
	if ((glm::dot(glm::normalize(glm::vec2(gameObject->transform->front)), toPoint) < 0 && glm::distance(points[pointToCheck]->transform->position, pos) < 6) || glm::distance(points[pointToCheck]->transform->position, pos) < carSize * ((carMovement->getActSpeed() * 4.0f / carMovement->getMaxSpeed()) + 2))
		return true;

	return false;
}

bool MedBot::HandlePredictions()
{
	glm::vec3 futurePos = carMovement->getFuturePos() * (1.0f / Time::deltaTime);
	if (MovedOverPoint(gameObject->transform->position + futurePos)) {
		currentPoint = (currentPoint + 1) % points.size();
		changedPoint = true;
	}

	toPoint = glm::normalize(glm::vec2(points[currentPoint]->transform->position - (gameObject->transform->position + futurePos)));

	bool colli = HandleCollision();

	return colli;
}

bool MedBot::HandleCollision()
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

	if (carMovement->getSurface() == 0)
		nextPointDot = glm::dot(glm::normalize(glm::vec2(-gameObject->transform->right)), toPoint);

	if ((avoiding == 1 && coll->cm == nullptr ) || (nextPointDot > 0)) {
		carMovement->makeLeftTurn();
		avoiding = 1;
	}
	else if ((avoiding == -1 && coll->cm == nullptr) || (nextPointDot < 0)) {
		carMovement->makeRightTurn();
		avoiding = -1;
	}
	return true;
}

bool MedBot::HandleCheckPoint()
{
	if (glm::length(glm::vec2(points[currentPoint]->transform->position - gameObject->transform->position)) > glm::length(map->GetCheckPoint(thisCar->checkpoint + 1)->transform->position - gameObject->transform->position)) {
		toPoint = glm::normalize(glm::vec2(map->GetCheckPoint(thisCar->checkpoint + 1)->transform->position - gameObject->transform->position));
		return true;
	}

	return false;
}