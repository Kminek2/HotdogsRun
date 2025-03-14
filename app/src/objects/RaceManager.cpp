#include "objects/RaceManager.hpp"
#include "Application.h"
#include "objects/ShowOBB.h"

#include <algorithm>
#include <glm/vec3.hpp>
#include <set>

const std::array<glm::vec2, 4> RaceManager::offsets =
    std::array<glm::vec2, 4>({glm::vec2(1, 1), glm::vec2(-1, 1), glm::vec2(-1, -1), glm::vec2(1, -1)});

RaceManager *RaceManager::SetMapManager(MapManager *map_manager) {
	this->map_manager = map_manager;
	return this;
}

RaceManager *RaceManager::SetCarsRelativeOffset(float offset) {
	this->cars_relative_offset = offset;
	return this;
}

/// <summary>
/// Return n normalized to [0, lim)
/// </summary>
int normalize(int n, int lim) {
	while (n < 0)
		n += lim;
	return n % lim;
}

/// <summary>
/// Place the car in the right starting position. Returns the id of the car
/// </summary>
RaceManager *RaceManager::AddCar(GameObject *car) {
	if (!map_manager)
		throw std::invalid_argument("init map_manager first");

	if (!car->GetOBBsCount())
		car->AddDefaultOBB();

	const int n = map_manager->GetLen();
	int point_id = normalize(-cars_placed / 4, n);

	glm::vec3 tile_points[2] = {map_manager->GetPoint(point_id)->transform->position,
				    map_manager->GetPoint(normalize(point_id + 1, n))->transform->position};

	glm::vec2 tile_points_transformed[2] = {glm::vec2(tile_points[0].x, tile_points[0].y), glm::vec2(tile_points[1].x, tile_points[1].y)};

	float orient =
	    atan2(tile_points_transformed[1].y - tile_points_transformed[0].y, tile_points_transformed[1].x - tile_points_transformed[0].x);

	car->transform->MoveTo(glm::vec3(tile_points_transformed[0].x, tile_points_transformed[0].y, 1));
	car->transform->RotateTo(glm::vec3(0, 0, glm::degrees(orient) - 180.0f));

	glm::vec2 offset = offsets[cars_placed % 4] * map_manager->GetMapScale() * map_manager->GetMapTileSize() * cars_relative_offset;
	car->transform->Translate(glm::vec3(offset.x, offset.y, 0));

	++cars_placed;
	car_objects.push_back(new RaceManager::CarObject(car, 0, 0));
	return this;
}

void RaceManager::OnCheckpoint(Collisions::CollisionData *collision_data) {
	GameObject *car = nullptr, *cp = nullptr;

	if (collision_data->obj1->GetModelName() == "checkpoint") {
		car = collision_data->obj2;
		cp = collision_data->obj1;
	} else {
		car = collision_data->obj1;
		cp = collision_data->obj2;
	}

	CarObject *car_obj = *std::find_if(car_objects.begin(), car_objects.end(), [car](CarObject *a) { return a->car == car; });

	if (map_manager->GetCheckPoint(normalize(car_obj->checkpoint + 1, map_manager->GetCheckPoints())) != cp)
		return;

	car_obj->checkpoint++;
	car_obj->time = Time::lastTime;

	std::cout << car->GetModelName() << " reached " << car_obj->checkpoint << "-th chekpoint at " << car_obj->time << '\n';

	if (termination_condition == LAPS && (car_obj->checkpoint / map_manager->GetCheckPoints()) >= termination_condition_value)
		EndRace();
}

/// <summary>
/// Start race. Has to have at least two cars and a termination condition.
/// </summary>
void RaceManager::StartRace() {
	if (car_objects.size() < 2)
		throw std::invalid_argument("add more cars");
	if (termination_condition == undefined)
		throw std::invalid_argument("define a condition");

	StartAnimation();

	race_started = true;
	std::cout << "Race stared with the termination condition of " << (termination_condition == TIME ? "TIME" : "LAPS") << " ("
		  << termination_condition_value << ")\n";

	if (termination_condition == TIME)
		Application::Invoke(&RaceManager::EndRace, termination_condition_value, this, true);

	std::set<std::string> car_names;

	for (int i = 0; i < map_manager->GetCheckPoints(); i++) {
		map_manager->GetCheckPoint(i)->AddDefaultOBB(glm::vec3(1), true)->surface_type = NEVER_COLLIDE;
	}

	for (CarObject *car : car_objects)
		if (car_names.insert(car->car->GetModelName()).second) {
			Collisions::addCallback("checkpoint", car->car->GetModelName(),
						std::bind(&RaceManager::OnCheckpoint, this, std::placeholders::_1));
			std::cout << "Added callback to [checkpoint - " << car->car->GetModelName() << "] collision\n";
		}
}

/// <summary>
/// Sets the termination condition. If the condition is LAPS, val is the number of laps to win. If
/// the condition is TIME, val is the number of miliseconds to end the race.
/// </summary>
RaceManager *RaceManager::SetEndCondition(TerminationCondition condition, unsigned long long val) {
	if (condition == undefined)
		throw std::invalid_argument("...really?");

	termination_condition = condition;
	termination_condition_value = val;

	return this;
}

/// <summary>
/// Force-end the race. If executeCallbacks is set to false, the subscribers will not be called.
/// Also used by the main gameplay loop.
/// </summary>
RaceManager::CarObject *RaceManager::EndRace(bool executeCallbacks) {
	if (!race_started)
		return nullptr; // race might have been force-ended already

	std::sort(car_objects.begin(), car_objects.end(), [](CarObject *a, CarObject *b) {
		if (a->checkpoint == b->checkpoint)
			return a->time < b->time;
		return a->checkpoint > b->checkpoint;
	});

	std::cout << "Race ended.\n";

	if (!executeCallbacks)
		return car_objects[0];

	for (const auto &sub : subscribers)
		sub(car_objects[0]);

	return car_objects[0];
}

/// <summary>
/// Subscribe a `void` return type function to the 'race end' event
/// </summary>
void RaceManager::SubscribeToRaceEnd(const std::function<void(CarObject *)> &callback) { subscribers.push_back(callback); }

RaceManager *RaceManager::SetAnimationManager(AnimationManager *am) {
	animation_manager = am;
	return this;
}

void RaceManager::StartAnimation() {
	assert(animation_manager != nullptr);

	animation_manager->addToQueue({{car_objects[0]->car->transform->position - glm::vec3({10.8015f, -8.80438f, -6.55353f}),
					{car_objects[0]->car->transform->rotation.x - -320.0f, car_objects[0]->car->transform->rotation.y - 18.25f}},
				       {car_objects[0]->car->transform->position - glm::vec3({10.8015f, -8.80438f, -6.55353f}),
					{car_objects[0]->car->transform->rotation.x - -320.0f, car_objects[0]->car->transform->rotation.y - 18.25f}},
				       5.0f,
				       {0.0f, 0.0f, 0.0f},
				       false});
	animation_manager->addToQueue({{car_objects[0]->car->transform->position - glm::vec3({10.8015f, -8.80438f, -6.55353f}),
					{car_objects[0]->car->transform->rotation.x - -320.0f, car_objects[0]->car->transform->rotation.y - 18.25f}},
				       {car_objects[0]->car->transform->position - glm::vec3({4.96434f, -4.10972f, -4.14516f}),
					{car_objects[0]->car->transform->rotation.x - -320.0f, car_objects[0]->car->transform->rotation.y - 21.25f}},
				       1.5f,
				       {0.0f, 0.0f, 0.0f},
				       false});
	animation_manager->addToQueue({{car_objects[0]->car->transform->position - glm::vec3({4.96434f, -4.10972f, -4.14516f}),
					{car_objects[0]->car->transform->rotation.x - -320.0f, car_objects[0]->car->transform->rotation.y - 21.25f}},
				       {car_objects[0]->car->transform->position - glm::vec3({6.03574f, 3.54317f, -3.74811f}),
					{car_objects[0]->car->transform->rotation.x - -37.25f, car_objects[0]->car->transform->rotation.y - 19.25f}},
				       2.0f,
				       {0.0f, 0.0f, 0.0f},
				       false});
	animation_manager->addToQueue({{car_objects[0]->car->transform->position - glm::vec3({6.03574f, 3.54317f, -3.74811f}),
					{car_objects[0]->car->transform->rotation.x - -37.25f, car_objects[0]->car->transform->rotation.y - 19.25f}},
				       {car_objects[0]->car->transform->position - glm::vec3({5.01428f, -5.82023f, -1.38018f}),
					{car_objects[0]->car->transform->rotation.x - -275.75f, car_objects[0]->car->transform->rotation.y - 2.0f}},
				       2.0f,
				       {0.0f, 0.0f, 0.0f},
				       false});
	animation_manager->addToQueue({{car_objects[0]->car->transform->position - glm::vec3({5.01428f, -5.82023f, -1.38018f}),
					{car_objects[0]->car->transform->rotation.x - -275.75f, car_objects[0]->car->transform->rotation.y - 2.0f}},
				       {car_objects[0]->car->transform->position - glm::vec3({-3.1062f, -6.63792f, -1.38018f}),
					{car_objects[0]->car->transform->rotation.x - -269.0f, car_objects[0]->car->transform->rotation.y - -0.25f}},
				       1.5f,
				       {0.0f, 0.0f, 0.0f},
				       false});
	animation_manager->addToQueue({{car_objects[0]->car->transform->position - glm::vec3({-3.1062f, -6.63792f, -1.38018f}),
					{car_objects[0]->car->transform->rotation.x - -269.0f, car_objects[0]->car->transform->rotation.y - -0.25f}},
				       {car_objects[0]->car->transform->position - glm::vec3({-15.0351f, 0.0f, -5.47232f}),
					{car_objects[0]->car->transform->rotation.x - -180.0f, car_objects[0]->car->transform->rotation.y - 20.0f}},
				       1.5f,
				       {5.0f, 5.0f, 1.0f},
				       true});
}