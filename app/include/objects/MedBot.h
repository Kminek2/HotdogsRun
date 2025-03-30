#pragma once
#include "CarMovement.h"
#include "MapManager.h"
#include "ObjectScript.h"
#include "RaceManager.hpp"
class MedBot : public ObjectScript
{
private:
	int avoiding;
	CarMovement* carMovement;
	const MapManager* map;
	const std::vector<GameObject*> points;
	const float breakMult;
	const float breakPower;
	const float prefaredSpeed;
	float carSize;

	const RaceManager::CarObject* thisCar;

	uint32_t currentPoint;
	bool changedPoint;

	glm::vec2 toPoint;

	GameObject* antiCollider;
	float lastCollided;
	glm::vec3 lastCollPos;

	bool shouldReverse;

	bool MovedOverPoint(glm::vec3 pos, int previous = 0);
	bool HandlePredictions();
	bool HandleCollision();
	bool HandleCheckPoint();
public:
	MedBot(CarMovement* carMovement, MapManager* map, RaceManager::CarObject* thisCarObj, float prefaredSpeed = 0.6f, float breakPower = 0.4f, float brakMult = 0.6);

	void Init() override;
	void EarlyUpdate() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;

	MedBot* copy() override { return new MedBot(*this); }
};

