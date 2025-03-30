#pragma once
#include "ObjectScript.h"
#include "objects/CarMovement.h"
#include "objects/MapManager.h"
#include "objects/RaceManager.hpp"
class AntiPersonelBot :
    public ObjectScript
{
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
	bool shouldReverse;
	float lastCollided;
	glm::vec3 lastCollPos;

	glm::vec2 toPoint;

	GameObject* antiCollider;

	GameObject* mainCar;

	bool MovedOverPoint(glm::vec3 pos, int previous = 0);
	bool HandlePredictions();
	bool HandleCollision();
	bool HandleCheckPoint();
	bool HandleKill();
public:
	AntiPersonelBot(CarMovement* carMovement, MapManager* map, RaceManager::CarObject* thisCarObj, GameObject* mainCar, float breakPower = 0.2f, float brakMult = 0.3);

	void Init() override;
	void EarlyUpdate() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;


	AntiPersonelBot* copy() override { return new AntiPersonelBot(*this); }
};

