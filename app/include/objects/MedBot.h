#pragma once
#include "CarMovement.h"
#include "MapManager.h"
#include "ObjectScript.h"
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

	uint32_t currentPoint;
	bool changedPoint;

	glm::vec2 toPoint;

	GameObject* antiCollider;

	bool MovedOverPoint(glm::vec3 pos, int previous = 0);
	bool HandlePredictions();
	bool HandleCollision();
public:
	MedBot(CarMovement* carMovement, MapManager* map, float prefaredSpeed = 0.7f, float breakPower = 0.6f, float brakMult = 0.5);

	void Init() override;
	void EarlyUpdate() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;
};

