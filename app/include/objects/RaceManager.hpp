#pragma once

#include "MapManager.h"
#include "GameObject.h"
#include "Collisions.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <array>
#include <functional>
#include <set>
#include "objects/AnimationManager.h"
#include "objects/CarMovement.h"

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

	struct LiveRaceObject {
		GameObject* car;
		double progress;
	};

	RaceManager* SetMapManager(MapManager* map_manager);
	RaceManager* SetCarsRelativeOffset(float offset);
	RaceManager* AddCar(GameObject* car);
	RaceManager* SetAnimationManager(AnimationManager* am);

	void Update();
	
	void handleClock();
	Text* clock = nullptr;

	void StartRace();
	RaceManager* SetEndCondition(TerminationCondition condition, unsigned long long val);
	CarObject* EndRace(bool executeCallbacks = true);

	void OnCheckpoint(Collisions::CollisionData* collision_data);
	void SubscribeToRaceEnd(const std::function<void(CarObject*)>& callback);

	std::vector<LiveRaceObject*> GetLiveRace();
	std::set<std::string> GetCarNames();

private:
	MapManager* map_manager = nullptr;
	double avg_cp_dist = -1;

	int cars_placed = 0;

	std::vector<CarObject*> car_objects;
	static std::set<std::string> car_names;

	bool race_started = false;

	float cars_relative_offset = .25f;
	const static std::array<glm::vec2, 5> offsets;
	float race_time_elapsed = 0.0f;

	TerminationCondition termination_condition = undefined;
	unsigned long long termination_condition_value;

	std::vector<std::function<void(CarObject*)>> subscribers;

	AnimationManager* animation_manager = nullptr;

	GameObject* countdown_number = nullptr;

	void StartAnimation();
	void AfterCountdown();

	void CalcAvgDist();
};