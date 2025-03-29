#include "objects/AntiPersonelBot.h"
#include "objects/ShowOBB.h"

AntiPersonelBot::AntiPersonelBot(CarMovement* carMovement, MapManager* map, RaceManager::CarObject* thisCarObj, GameObject* mainCar, float breakPower, float brakMult) : carMovement(carMovement), map(map), points(*map->GetPoints()), breakMult(brakMult), breakPower(breakPower), thisCar(thisCarObj)
{
	currentPoint = 1;
	toPoint = glm::vec3(0);
	avoiding = 0;
	changedPoint = false;
	shouldReverse = false;
	lastCollided = 100;
	this->mainCar = mainCar;
}

void AntiPersonelBot::Init()
{
	carSize = gameObject->GetModelMaxDistVert()[0].x - gameObject->GetModelMaxDistVert()[0].y;
	glm::vec3 obbSizes = glm::vec3(carSize * 3, gameObject->GetModelMaxDistVert()[1].x - gameObject->GetModelMaxDistVert()[1].y, gameObject->GetModelMaxDistVert()[2].x - gameObject->GetModelMaxDistVert()[2].y) * 1.1f;

	antiCollider = new GameObject("", gameObject->transform->position, gameObject->transform->rotation, gameObject->transform->scale, NEVER_COLLIDE);
	antiCollider->addOBB(*new OBB({ 0, 0, 0 }, obbSizes / 2.0f));
}

void AntiPersonelBot::EarlyUpdate()
{
	lastCollided += Time::deltaTime;
	if (shouldReverse) {
		carMovement->goBackwards();
		return;
	}
	bool changesToPoint = changedPoint;
	changedPoint = false;
	if (HandlePredictions()) {
		return;
	}

	avoiding = 0;

	float nextPointDot = glm::dot(glm::normalize(glm::vec2(gameObject->transform->front)), toPoint);
	glm::vec3 fromLastPoint = glm::normalize(points[glm::normalize((long long)currentPoint - 1, (long long)points.size())]->transform->position - points[glm::normalize((long long)currentPoint - 2, (long long)points.size())]->transform->position);

	if ((changesToPoint || changedPoint) && !HandleKill()) {
		glm::vec2 tmpToPoint = toPoint;
		toPoint = glm::normalize((glm::vec2((points[glm::normalize((long long)currentPoint - 1, (long long)points.size())]->transform->position + fromLastPoint) - (gameObject->transform->position))));
		if (!MovedOverPoint(gameObject->transform->position + gameObject->transform->front * 3.0f, 1))
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

	if (abs(1 - nextPointDot) > breakMult && carMovement->getActSpeed() / carMovement->getMaxSpeed() > glm::max(((nextPointDot + 1) * 0.25f) / (breakPower * 10.0f), breakPower))
		carMovement->useHandBreak();
	else {
		carMovement->goForward();
		float nextPointDot2 = glm::dot(glm::normalize(glm::vec2(gameObject->transform->front)), glm::normalize(glm::vec2(points[(currentPoint + 1) % points.size()]->transform->position - gameObject->transform->position)));
		float nextPointDot3 = glm::dot(glm::normalize(glm::vec2(gameObject->transform->front)), glm::normalize(glm::vec2(points[(currentPoint + 2) % points.size()]->transform->position - gameObject->transform->position)));
		if (abs(1 - nextPointDot) < breakMult && abs(1 - thisPointDot) < breakMult && abs(1 - nextPointDot2) < breakMult / 600.0f && abs(1 - nextPointDot3) < breakMult / 600.0f)
			carMovement->useNitro();
	}

	nextPointDot = glm::dot(glm::normalize(glm::vec2(gameObject->transform->right)), nextToPoint);
	float forwardDot = glm::dot(glm::normalize(glm::vec2(gameObject->transform->right)), nextToPoint);

	if (nextPointDot < -breakMult * glm::max(abs(carMovement->getAxleAngle() / 10.0f), 0.0001f)) {
		carMovement->makeLeftTurn();
	}
	else if (nextPointDot > breakMult * glm::max(abs(carMovement->getAxleAngle() / 10.0f), 0.0001f)) {
		carMovement->makeRightTurn();
	}
	else if (forwardDot < 0) {
		if (nextPointDot <= 0) {
			carMovement->makeLeftTurn();
		}
		else {
			carMovement->makeRightTurn();
		}
	}
}

void AntiPersonelBot::Update()
{
}

void AntiPersonelBot::LateUpdate()
{
	antiCollider->obbs[0].sizes = glm::vec3(carSize, gameObject->GetModelMaxDistVert()[1].x - gameObject->GetModelMaxDistVert()[1].y, gameObject->GetModelMaxDistVert()[2].x - gameObject->GetModelMaxDistVert()[2].y) * glm::vec3(4.0f * carMovement->getActSpeed() / carMovement->getMaxSpeed() + 1.0f, 1.1f, 1.1f);
	antiCollider->transform->MoveTo(gameObject->transform->position + gameObject->transform->front * carSize * (4.0f * carMovement->getActSpeed() / carMovement->getMaxSpeed() + 1));
	antiCollider->transform->RotateTo(gameObject->transform->rotation);
	antiCollider->transform->ScaleTo(gameObject->transform->scale);

	if (carMovement->getDidColide() && lastCollided < 5.0f)
		shouldReverse = true;
	else if (carMovement->getDidColide())
		lastCollided = 0;
	else if (shouldReverse && lastCollided > 3.0f)
		shouldReverse = false;

	if (lastCollided > 5.0f)
		avoiding = 0;
}

void AntiPersonelBot::OnDestroy()
{
	delete antiCollider;
}

bool AntiPersonelBot::MovedOverPoint(glm::vec3 pos, int previous)
{
	long long pointToCheck = glm::normalize((long long)currentPoint - previous, (long long)points.size());
	glm::vec3 toPointFromLast = points[pointToCheck]->transform->position - points[glm::normalize((long long)pointToCheck - 1, (long long)points.size())]->transform->position;
	glm::vec3 toNextPoint = points[glm::normalize((long long)pointToCheck + 1, (long long)points.size())]->transform->position - points[pointToCheck]->transform->position;
	float ang = (glm::dot(glm::normalize(glm::vec2(toPointFromLast)), glm::normalize(glm::vec2(toNextPoint))) - 1) * -0.5f;
	if ((glm::dot(glm::normalize(glm::vec2(gameObject->transform->front)), glm::normalize(glm::vec2(points[currentPoint]->transform->position - (gameObject->transform->position)))) < 0 && glm::distance(points[pointToCheck]->transform->position, pos) < 12) || glm::distance(points[pointToCheck]->transform->position, pos) < carSize * 2.0f * (ang + 1))
		return true;

	return false;
}

bool AntiPersonelBot::HandlePredictions()
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

bool AntiPersonelBot::HandleCollision()
{
	GameObject* coll = nullptr;

	for (GameObject* obj : GameObject::getAllGameObjects()) {
		if (obj->surface_type == ALWAYS_COLLIDE && obj != gameObject && obj != mainCar && Collisions::checkCollision(*obj, *antiCollider) && (coll == nullptr || glm::distance(obj->transform->position, gameObject->transform->position) < glm::distance(coll->transform->position, gameObject->transform->position))) {
			coll = obj;
		}
	}

	if (coll == nullptr)
		return false;
	else if (shouldReverse)
	{
		carMovement->goBackwards();
		return true;
	}

	if (carMovement->getActSpeed() / carMovement->getMaxSpeed() < breakMult * 0.5f && !shouldReverse)
		carMovement->goForward();
	else if (!shouldReverse && coll->cm == nullptr)
		carMovement->useHandBreak();

	glm::vec2 toColl = glm::normalize(glm::vec2(coll->transform->position - gameObject->transform->position));
	float nextPointDot = glm::dot(glm::normalize(glm::vec2(gameObject->transform->right)), toColl);

	if (carMovement->getSurface() == 0)
		nextPointDot = glm::dot(glm::normalize(glm::vec2(-gameObject->transform->right)), toPoint);

	if ((avoiding == 1 && coll->cm == nullptr) || (nextPointDot > 0)) {
		carMovement->makeLeftTurn();
		if(coll->cm != nullptr)
			avoiding = 1;
	}
	else if ((avoiding == -1 && coll->cm == nullptr) || (nextPointDot < 0)) {
		carMovement->makeRightTurn();
		if (coll->cm != nullptr)
			avoiding = -1;
	}
	return true;
}

bool AntiPersonelBot::HandleCheckPoint()
{
	if ((currentPoint - 1) % map->cp_offset == 0 && glm::distance(glm::vec2(map->GetCheckPoint(thisCar->checkpoint + 1)->transform->position), (glm::vec2)gameObject->transform->position) < carSize * 2.0f) {
		toPoint = glm::normalize(glm::vec2(map->GetCheckPoint(thisCar->checkpoint + 1)->transform->position - gameObject->transform->position));
		return true;
	}

	return false;
}

bool AntiPersonelBot::HandleKill()
{
	glm::vec2 toMain = glm::vec2(mainCar->transform->position - gameObject->transform->position);
	if (glm::dot(glm::normalize(toMain), glm::normalize((glm::vec2)gameObject->transform->front)) < 0.4f || glm::dot(glm::normalize(toMain), toPoint) < 0 || glm::length(toMain) > carSize * 2.0f)
		return false;

	toPoint = glm::normalize(toMain);
	return true;
}
