#pragma once

#include "MapManager.h"
#include "GameObject.h"
#include "Collisions.h"
#include "UiObject.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <array>
#include <functional>
#include <set>
#include "objects/AnimationManager.h"
#include "objects/CarMovement.h"

class RaceManager {
public:
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
	CarObject* AddCar(GameObject* car, bool main = false);
	RaceManager* SetAnimationManager(AnimationManager* am);

	void Update();
	
	void handleClock();
	void handleVelocityDisplay();
	void handleLoops();
	void handleNitros();
	void handleTracking();

	void StartRace();
	RaceManager* SetEndCondition(unsigned long long laps);
	CarObject* EndRace(bool executeCallbacks = true);

	void OnCheckpoint(Collisions::CollisionData* collision_data);
	void SubscribeToRaceEnd(const std::function<void(CarObject*)>& callback);

	std::vector<LiveRaceObject*> GetLiveRace(bool sorted = true);
	std::set<std::string> GetCarNames();
	std::vector<CarObject*>& getCarObjects(); 

	GameObject* getMainCar();

private:
	MapManager* map_manager = nullptr;

	double sum_x = 0;
	double avg_cp_dist = -1;

	int cars_placed = 0;

	GameObject* main_car = nullptr;
	std::vector<CarObject*> car_objects;
	static std::set<std::string> car_names;

	bool race_started = false;
	bool race_ended = false;

	float cars_relative_offset = .25f;
	const static std::array<glm::vec2, 5> offsets;
	float race_time_elapsed = 0.0f;
	const double VELOCITY_DISPLAY_MULTIPLIER = 5;

	unsigned long long termination_condition_value; // aka laps

	std::vector<std::function<void(CarObject*)>> subscribers;

	AnimationManager* animation_manager = nullptr;

	GameObject* countdown_number = nullptr;

	void StartAnimation();
	void AfterCountdown();

	void CalcAvgDist();

	Text* clock = nullptr;
	Text* loop_tracker = nullptr;
	Text* velocity = nullptr;

	Text* place = nullptr;

	Sprite* progress_bar = nullptr;
	std::vector<Sprite*> checkpoint_trackers;
	std::vector<Sprite*> race_trackers;

	Sprite* nitro_icon = nullptr;
	Text* nitro_counter = nullptr;
};