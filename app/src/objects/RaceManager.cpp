#include "objects/RaceManager.hpp"
#include "Application.h"
#include "objects/CarMovement.h"
#include "objects/ShowOBB.h"

#include "StringOperations.hpp"

#include <algorithm>
#include <glm/vec3.hpp>

const std::array<glm::vec2, 5> RaceManager::offsets = std::array<glm::vec2, 5>({
	glm::vec2(2.0, 1.5),
	glm::vec2(2.0, 0.75),
	glm::vec2(2.0, 0),
	glm::vec2(2.0, -0.75),
	glm::vec2(2.0, -1.5)
});

std::set<std::string> RaceManager::car_names;

RaceManager *RaceManager::SetMapManager(MapManager *map_manager) {
	this->map_manager = map_manager;
	return this;
}

RaceManager *RaceManager::SetCarsRelativeOffset(float offset) {
	this->cars_relative_offset = offset;
	return this;
}

/// <summary>
/// Place the car in the right starting position.
/// </summary>
RaceManager *RaceManager::AddCar(GameObject *car, bool main) {
	if (!map_manager)
		throw std::invalid_argument("init map_manager first");

	if (!car->GetOBBsCount())
		car->AddDefaultOBB();

	const int n = map_manager->GetLen();
	int point_id = glm::normalize(-cars_placed / 5, n);

	glm::vec3 tile_points[2] = {map_manager->GetPoint(point_id)->transform->position,
					map_manager->GetPoint(glm::normalize(point_id + 1, n))->transform->position};

	glm::vec2 tile_points_transformed[2] = {glm::vec2(tile_points[0].x, tile_points[0].y), glm::vec2(tile_points[1].x, tile_points[1].y)};

	float orient =
		atan2(tile_points_transformed[1].y - tile_points_transformed[0].y, tile_points_transformed[1].x - tile_points_transformed[0].x);

	car->transform->MoveTo(glm::vec3(tile_points_transformed[0].x, tile_points_transformed[0].y, 0.0f));
	car->transform->RotateTo(glm::vec3(0, 0, glm::degrees(orient) - 180.0f));

	glm::vec2 offset = offsets[cars_placed % 5] * map_manager->GetMapScale() * map_manager->GetMapTileSize() * cars_relative_offset;
	car->transform->Translate(glm::vec3(offset.x, offset.y, 0));

	++cars_placed;
	car_objects.push_back(new RaceManager::CarObject(car, 0, 0));
	car_names.insert(car->GetModelName());

	if (main) {
		main_car = car;
		std::swap(car_objects[0], car_objects[cars_placed - 1]);
	}

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

	if (map_manager->GetCheckPoint(car_obj->checkpoint + 1) != cp)
		return;

	car_obj->checkpoint++;
	car_obj->time = Time::lastTime;

	if (car_obj->checkpoint / map_manager->GetCheckPoints() >= termination_condition_value)
		EndRace();
}

/// <summary>
/// Start race. Has to have at least two cars and a termination condition.
/// </summary>
void RaceManager::StartRace() {
	if (!main_car)
		throw std::invalid_argument("you didn't specify the main car");
	if (car_objects.size() < 2)
		throw std::invalid_argument("add more cars");	
	
	const glm::vec4 color = glm::vec4(1, 1, 1, .4);
	const float height = .025;
	const glm::vec3 pos = glm::vec3(0, 1 - 2 * height, 0);
	const float scale = 1.1;

	race_trackers.reserve(cars_placed);

	Sprite* sp = new Sprite("tracker_main", color);
	sp->rectTransform->SetHeight(height)->ScaleTimes(scale)->MoveTo(pos);

	race_trackers.push_back(sp);

	for (int _ = 1; _ < cars_placed; _++) {
		Sprite* sp = new Sprite("tracker_norm", color);
		sp->rectTransform->SetHeight(height)->ScaleTimes(scale)->MoveTo(pos);

		race_trackers.push_back(sp);
	}

	progress_bar = new Sprite("progress", color);
	progress_bar->rectTransform->SetWidth(.75, false)->SetHeight(height, false)->MoveTo(pos);

	StartAnimation();
}

void RaceManager::AfterCountdown() {
	if (countdown_number)
		delete countdown_number;
	countdown_number = nullptr;
	
	CarMovement::disabled_inputs = false;

	race_started = true;

	for (int i = 0; i < map_manager->GetCheckPoints(); i++) {
		map_manager->GetCheckPoint(i)->AddDefaultOBB(glm::vec3(1), true)->surface_type = NEVER_COLLIDE;
	}

	for (const std::string &car_name : car_names) {
	}

	clock = new Text("HackBold", {0.95, 0.95, 0.1}, {1, 1}, 0.3f);
	clock->SetText("Get ready!");

	velocity = new Text("HackBold", { -.95,.95,.1 }, { -1,1 }, .3);
	velocity->SetText("--");

	loop_tracker = new Text("HackBold", {0.95, 0.75, 0.1}, {1, 1}, 0.3f);
	loop_tracker->SetText("-/-");
}

void RaceManager::CalcAvgDist()
{
	double sum_x = 0;
	unsigned n = map_manager->GetCheckPoints();

	for (unsigned i = 0; i < n; i++)
		sum_x += Collisions::getL1Distance(map_manager->GetCheckPoint(i), map_manager->GetCheckPoint(i + 1));

	avg_cp_dist = sum_x / n;
}

/// <summary>
/// Sets the termination condition. If the condition is LAPS, val is the number of laps to win.
/// </summary>
RaceManager *RaceManager::SetEndCondition(unsigned long long laps) {
	termination_condition_value = laps;
	return this;
}

/// <summary>
/// Force-end the race. If executeCallbacks is set to false, the subscribers will not be called.
/// Also used by the main gameplay loop.
/// </summary>
RaceManager::CarObject *RaceManager::EndRace(bool executeCallbacks) {
	if (!race_started)
		return nullptr; // race might have been force-ended already

	car_names.clear();

	race_ended = true;

	delete clock;
	delete loop_tracker;

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

	CarMovement::disabled_inputs = true;

	Sprite* bg = new Sprite("end_race_bg");
	bg->rectTransform->ScaleTo(glm::vec2(100.0f, 100.0f));
	bg->rectTransform->MoveTo(glm::vec3(0.0f, 0.0f, 1.0f));

	Text* ent = new Text("SansSerif", glm::vec3(0.1f, -0.8f, 0.0f), glm::vec2(0.0f));
	ent->SetText("Press ENTER to continue!");
	ent->ChangeSize(0.2);
	
	return car_objects[0];
}

/// <summary>
/// Subscribe a `void` return type function to the 'race end' event
/// </summary>
void RaceManager::SubscribeToRaceEnd(const std::function<void(CarObject *)> &callback) { subscribers.push_back(callback); }

/// <summary>
/// Kinda heavy function. Don't use every frame (please)
/// </summary>
std::vector<RaceManager::LiveRaceObject*> RaceManager::GetLiveRace(bool sorted)
{
	std::vector<LiveRaceObject*> live_race;
	live_race.reserve(car_objects.size());

	unsigned cp_count = map_manager->GetCheckPoints();

	if (avg_cp_dist == -1) CalcAvgDist();

	double Δp = 1 / static_cast<double>(cp_count * termination_condition_value);

	// yes, i could write this in one line, but this is *a bit* more readible
	for (CarObject* car : car_objects) {
		float checkpoint_progress = car->checkpoint 
			+ Collisions::getL1Distance(car->car, map_manager->GetCheckPoint(car->checkpoint + 1)) / avg_cp_dist;

		live_race.push_back(new LiveRaceObject(car->car, checkpoint_progress * Δp));
	}

	if(sorted)
		std::sort(live_race.begin(), live_race.end(), [](LiveRaceObject* a, LiveRaceObject* b) {return a->progress > b->progress; });

	return live_race;
}

std::set<std::string> RaceManager::GetCarNames() { return car_names; }

RaceManager *RaceManager::SetAnimationManager(AnimationManager *am) {
	animation_manager = am;
	return this;
}

void RaceManager::StartAnimation() {
	assert(animation_manager);

	animation_manager->addToQueue({{main_car->transform->position - glm::vec3({10.8015f, -8.80438f, -6.55353f}),
					{main_car->transform->rotation.x - -320.0f, main_car->transform->rotation.y - 18.25f}},
					   {main_car->transform->position - glm::vec3({10.8015f, -8.80438f, -6.55353f}),
					{main_car->transform->rotation.x - -320.0f, main_car->transform->rotation.y - 18.25f}},
					   5.0f,
					   {0.0f, 0.0f, 0.0f},
					   false,
					   [&]() {
						   CarMovement::disabled_inputs = true;
					   }});
	animation_manager->addToQueue({{main_car->transform->position - glm::vec3({10.8015f, -8.80438f, -6.55353f}),
					{main_car->transform->rotation.x - -320.0f, main_car->transform->rotation.y - 18.25f}},
					   {main_car->transform->position - glm::vec3({4.96434f, -4.10972f, -4.14516f}),
					{main_car->transform->rotation.x - -320.0f, main_car->transform->rotation.y - 21.25f}},
					   1.5f,
					   {0.0f, 0.0f, 0.0f},
					   false});
	animation_manager->addToQueue({{main_car->transform->position - glm::vec3({4.96434f, -4.10972f, -4.14516f}),
					{main_car->transform->rotation.x - -320.0f, main_car->transform->rotation.y - 21.25f}},
					   {main_car->transform->position - glm::vec3({6.03574f, 3.54317f, -3.74811f}),
					{main_car->transform->rotation.x - -37.25f, main_car->transform->rotation.y - 19.25f}},
					   2.0f,
					   {0.0f, 0.0f, 0.0f},
					   false});
	animation_manager->addToQueue({{main_car->transform->position - glm::vec3({6.03574f, 3.54317f, -3.74811f}),
					{main_car->transform->rotation.x - -37.25f, main_car->transform->rotation.y - 19.25f}},
					   {main_car->transform->position - glm::vec3({5.01428f, -5.82023f, -1.38018f}),
					{main_car->transform->rotation.x - -275.75f, main_car->transform->rotation.y - 2.0f}},
					   2.0f,
					   {0.0f, 0.0f, 0.0f},
					   false,
					   [&]() {
						   countdown_number =
						   new GameObject("3", main_car->transform->position + glm::vec3(-2.5f, -0.5f, 1.7f),
								  glm::vec3(90.0f, 0.0f, -90.0f), glm::vec3(0.15));
					   },
					   [&]() {
						   if (countdown_number != nullptr)
							   delete countdown_number;
						   countdown_number = nullptr;
					   }});
	animation_manager->addToQueue({{main_car->transform->position - glm::vec3({5.01428f, -5.82023f, -1.38018f}),
					{main_car->transform->rotation.x - -275.75f, main_car->transform->rotation.y - 2.0f}},
					   {main_car->transform->position - glm::vec3({-3.1062f, -6.63792f, -1.38018f}),
					{main_car->transform->rotation.x - -269.0f, main_car->transform->rotation.y - -0.25f}},
					   1.5f,
					   {0.0f, 0.0f, 0.0f},
					   false,
					   [&]() {
						   countdown_number =
						   new GameObject("2", main_car->transform->position + glm::vec3(-1.0f, 1.2f, 1.5f),
								  glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.15));
					   },
					   [&]() {
						   if (countdown_number != nullptr)
							   delete countdown_number;
						   countdown_number = nullptr;
					   }});
	animation_manager->addToQueue({{main_car->transform->position - glm::vec3({-3.1062f, -6.63792f, -1.38018f}),
					{main_car->transform->rotation.x - -269.0f, main_car->transform->rotation.y - -0.25f}},
					   {main_car->transform->position - glm::vec3({-15.0351f, 0.0f, -5.47232f}),
					{main_car->transform->rotation.x - -180.0f, main_car->transform->rotation.y - 20.0f}},
					   1.5f,
					   {5.0f, 5.0f, 1.0f},
					   true,
					   [&]() {
						   countdown_number =
						   new GameObject("1", main_car->transform->position + glm::vec3(2.0f, 1.0f, 2.0f),
								  glm::vec3(90.0f, 0.0f, 120.0f), glm::vec3(0.2));
					   },
					   std::bind(&RaceManager::AfterCountdown, this)});
}

double counter_000 = 0;
void RaceManager::Update() {
	if (!race_started || race_ended)
		return;

	handleClock();
	handleLoops();
	handleVelocityDisplay();

	counter_000 += Time::deltaTime;

	if (counter_000 >= .5f) {
		counter_000 -= .5f;
		handleTracking();
	}
}

void RaceManager::handleClock() {
	race_time_elapsed += Time::deltaTime;

	double whole;
	int milliseconds = static_cast<int>(std::modf(race_time_elapsed, &whole) * 1000);

	int total_seconds = static_cast<int>(whole);

	int minutes = total_seconds / 60;
	int seconds = total_seconds % 60;

	clock->SetText(String::padZeros(minutes, 2) + ':' + String::padZeros(seconds, 2) + '.' + String::padZeros(milliseconds, 3));
}

void RaceManager::handleVelocityDisplay() {
	std::string ntext = String::formatDouble(std::abs(main_car->cm->getActSpeed() * 10), 2) + " km/h";
	if(ntext != velocity->getText())
		velocity->SetText(ntext);
}

void RaceManager::handleLoops() {
	std::string ntext = (std::to_string((car_objects[0]->checkpoint / map_manager->GetCheckPoints()) + 1) + '/' + std::to_string(termination_condition_value));
	if (ntext != loop_tracker->getText())
		loop_tracker->SetText(ntext);
}

void RaceManager::handleTracking()
{
	std::vector<LiveRaceObject*> race_data = GetLiveRace(false);

	bool main_found = false;
	int i = 0;
	while (i < cars_placed) {
		int c_index = i + (!main_found);

		if (!main_found && race_data[i]->car == main_car) {
			main_found = true;
			c_index = 0;

			std::cout << "%: " << race_data[i]->progress << '\n';
		}

		glm::vec3 pos = race_trackers[c_index]->rectTransform->position;
		race_trackers[c_index]->rectTransform->MoveTo(glm::vec3(progress_bar->rectTransform->scale.x * (2 * race_data[i]->progress - 1), pos.y, pos.z));

		++i;
	}
}