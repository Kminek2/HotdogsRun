#pragma once
#include "ObjectScript.h"
#include "objects/CarMovement.h"
#include "objects/MapManager.h"
class StraightKingBot : public ObjectScript
{
private:
	int avoiding;
	CarMovement* carMovement;
	const MapManager* map;
	const std::vector<GameObject*> points;
	const float breakMult;
	const float breakPower;
	float carSize;

	uint32_t currentPoint;
	bool changedPoint;

	glm::vec2 toPoint;

	GameObject* antiCollider;

	bool MovedOverPoint(glm::vec3 pos, int previous = 0);
	bool HandlePredictions();
	bool HandleCollision();
public:
	StraightKingBot(CarMovement* carMovement, MapManager* map, float breakPower = 0.4f , float brakMult = 0.2);

	void Init() override;
	void EarlyUpdate() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;
};

