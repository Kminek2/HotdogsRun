#pragma once
#include "ObjectScript.h"
#include "objects/CarMovement.h"
#include "objects/MapManager.h"
#include "objects/RaceManager.hpp"
class StraightKingBot : public ObjectScript
{
private:
	int avoiding;
	CarMovement* carMovement;
	const MapManager* map;
	const std::vector<GameObject*> points;
	const float breakMult;
	const float breakPower;

	const RaceManager::CarObject* thisCar;
	float carSize;

	uint32_t currentPoint;
	bool changedPoint;

	glm::vec2 toPoint;

	GameObject* antiCollider;

	bool MovedOverPoint(glm::vec3 pos, int previous = 0);
	bool HandlePredictions();
	bool HandleCollision();
	bool HandleCheckPoint();
public:
	StraightKingBot(CarMovement* carMovement, MapManager* map, RaceManager::CarObject* thisCarObj, float breakPower = 0.4f , float brakMult = 0.2);

	void Init() override;
	void EarlyUpdate() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;
};

