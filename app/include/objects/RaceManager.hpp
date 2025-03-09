#pragma once

#include "MapManager.h"
#include "GameObject.h"

#include <glm/vec2.hpp>
#include <array>
#include <functional>

class RaceManager {
public:
	enum TerminationCondition {
		undefined, LAPS, TIME
	};

	struct CarObject {
		GameObject* car;
		unsigned long long lap;
		double time;
	};

	RaceManager* SetMapManager(MapManager* map_manager);
	RaceManager* SetCarsRelativeOffset(float offset);
	RaceManager* AddCar(GameObject* car);

	void StartRace();
	RaceManager* SetEndCondition(TerminationCondition condition, unsigned long long val);
	CarObject* EndRace(bool executeCallbacks = true);

	void SubscribeToRaceEnd(const std::function<void()>& callback);

private:
	MapManager* map_manager = nullptr;
	int cars_placed = 0;

	std::vector<GameObject*> cars;

	std::vector<CarObject*> car_objects;

	bool race_started = false;

	float cars_relative_offset = .25f;
	const static std::array<glm::vec2, 4> offsets;

	TerminationCondition termination_condition = undefined;
	unsigned long long termination_condition_value;

	std::vector<std::function<void()>> subscribers;
};