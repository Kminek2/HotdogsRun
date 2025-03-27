#pragma once
#include "ObjectScript.h"
#include "objects/CarMovement.h"
#include "objects/MapManager.h"
class StraightKingBot : public ObjectScript
{
private:
	CarMovement* carMovement;
	const MapManager* map;
	const std::vector<GameObject*> points;
	const float breakMult;
	float carSize;

	uint32_t currentPoint;

	glm::vec2 toPoint;

	GameObject* antiCollider;

	bool MovedOverPoint(glm::vec3 pos);
	bool HandlePredictions();
	bool HandleCollision();
public:
	StraightKingBot(CarMovement* carMovement, MapManager* map, float brakMult = 0.3);

	void Init() override;
	void EarlyUpdate() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;
};

