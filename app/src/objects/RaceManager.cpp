#include "objects/RaceManager.hpp"

#include <glm/vec3.hpp>
#include <algorithm>

const std::array<glm::vec2, 4> RaceManager::offsets = std::array<glm::vec2, 4>({ glm::vec2(1, 1), glm::vec2(-1, 1), glm::vec2(-1, -1), glm::vec2(1, -1) });

RaceManager* RaceManager::SetMapManager(MapManager* map_manager) {
	this->map_manager = map_manager;
	return this;
}

RaceManager* RaceManager::SetCarsRelativeOffset(float offset) {
	this->cars_relative_offset = offset;
	return this;
}

/// <summary>
/// Return n normalized to [0, lim)
/// </summary>
int normalize(int n, int lim) {
	while (n < 0) n += lim;
	return n % lim;
}

/// <summary>
/// Place the car in the right starting position. Returns the id of the car
/// </summary>
RaceManager* RaceManager::AddCar(GameObject* car) {
	if (!map_manager) throw std::invalid_argument("init map_manager first");

	const int n = map_manager->GetLen();
	int point_id = normalize(-cars_placed / 4, n);

	glm::vec3 tile_points[2] = {
		map_manager->GetPoint(point_id)->transform->position,
		map_manager->GetPoint(normalize(point_id + 1, n))->transform->position
	};

	glm::vec2 tile_points_transformed[2] = {
		glm::vec2(tile_points[0].x, tile_points[0].y),
		glm::vec2(tile_points[1].x, tile_points[1].y)
	};

	float orient = atan2(
		tile_points_transformed[1].y - tile_points_transformed[0].y,
		tile_points_transformed[1].x - tile_points_transformed[0].x
	);

	car->transform->MoveTo(glm::vec3(tile_points_transformed[0].x, tile_points_transformed[0].y, 1));
	car->transform->RotateTo(glm::vec3(0, 0, glm::degrees(orient) - 180.0f));

	glm::vec2 offset = offsets[cars_placed % 4] * map_manager->GetMapScale() * map_manager->GetMapTileSize() * cars_relative_offset;
	car->transform->Translate(glm::vec3(offset.x, offset.y, 0));

 	++cars_placed;
	cars.push_back(car);
	return this;
}

/// <summary>
/// Start race. Has to have at least two cars and a termination condition.
/// </summary>
void RaceManager::StartRace() {
	if (cars.size() < 2) throw std::invalid_argument("add more cars");
	if (termination_condition == undefined) throw std::invalid_argument("define a condition");

	car_objects.reserve(cars.size());
	std::transform(cars.begin(), cars.end(), car_objects.begin(), [](GameObject* car) {return new CarObject(car, 0, 0); });

	// add checkpoint handling logic here
}

/// <summary>
/// Sets the termination condition.
/// </summary>
RaceManager* RaceManager::SetEndCondition(TerminationCondition condition, unsigned long long val) {
	if (condition == undefined) throw std::invalid_argument("...really?");

	termination_condition = condition;
	termination_condition_value = val;

	return this;
}

/// <summary>
/// Force-end the race. If executeCallbacks is set to false, the subscribers will not be called
/// </summary>
RaceManager::CarObject* RaceManager::EndRace(bool executeCallbacks) {
	if (!race_started) throw std::invalid_argument("race hasn't even started bruh");

	std::sort(car_objects.begin(), car_objects.end(), [](CarObject* a, CarObject* b) {
		if (a->lap == b->lap)
			return a->time > b->time;
		return a->lap > b->lap;
	});

	if (!executeCallbacks) 
		return car_objects[0];

	for (const auto& sub : subscribers)
		sub();

	return car_objects[0];
}

/// <summary>
/// Subscribe a `void` return type function to the 'race end' event
/// </summary>
void RaceManager::SubscribeToRaceEnd(const std::function<void()>& callback) {
	subscribers.push_back(callback);
}