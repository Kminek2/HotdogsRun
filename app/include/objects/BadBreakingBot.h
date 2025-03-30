#pragma once
#include "CarMovement.h"
#include "MapManager.h"
#include "ObjectScript.h"
#include "RaceManager.hpp"
class BadBreakingBot : public ObjectScript
{
private:
	int avoiding;
	CarMovement* carMovement;
	const MapManager* map;
	const std::vector<GameObject*> points;
	const float breakMult;
	const float breakPower;
	float carSize;

	const RaceManager::CarObject* thisCar;

	uint32_t currentPoint;
	bool changedPoint;

	glm::vec2 toPoint;

	GameObject* antiCollider;
	float lastCollided;
	glm::vec3 lastCollPos;

	GameObject* mainCar;

	bool shouldReverse;

	RaceManager* race;

	bool breaking;

	bool MovedOverPoint(glm::vec3 pos, int previous = 0);
	bool HandlePredictions();
	bool HandleCollision();
	bool HandleCheckPoint();

	bool HandleKill();
public:
	BadBreakingBot(CarMovement* carMovement, MapManager* map, RaceManager::CarObject* thisCarObj, GameObject* mainCar, RaceManager* race, float breakPower = 0.4f, float brakMult = 0.3f);

	void Init() override;
	void EarlyUpdate() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;
};

