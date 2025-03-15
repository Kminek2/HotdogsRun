#pragma once

#include "MapManager.h"
#include "GameObject.h"
#include "Collisions.h"

#include <glm/vec2.hpp>
#include <array>
#include <functional>
#include "objects/AnimationManager.h"

class RaceManager {
public:
	enum TerminationCondition {
		undefined, LAPS, TIME
	};

	struct CarObject {
		GameObject* car;
		unsigned long long checkpoint;
		double time;
	};

	RaceManager* SetMapManager(MapManager* map_manager);
	RaceManager* SetCarsRelativeOffset(float offset);
	RaceManager* AddCar(GameObject* car);
	RaceManager* SetAnimationManager(AnimationManager* am);

	void StartRace();
	RaceManager* SetEndCondition(TerminationCondition condition, unsigned long long val);
	CarObject* EndRace(bool executeCallbacks = true);

	void OnCheckpoint(Collisions::CollisionData* collision_data);
	void SubscribeToRaceEnd(const std::function<void(CarObject*)>& callback);

private:
	MapManager* map_manager = nullptr;
	int cars_placed = 0;

	std::vector<CarObject*> car_objects;

	bool race_started = false;

	float cars_relative_offset = .25f;
	const static std::array<glm::vec2, 5> offsets;

	TerminationCondition termination_condition = undefined;
	unsigned long long termination_condition_value;

	std::vector<std::function<void(CarObject*)>> subscribers;

	AnimationManager* animation_manager = nullptr;

	void StartAnimation();
	void AfterCountdown();
};