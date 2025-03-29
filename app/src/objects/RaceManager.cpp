#include "objects/RaceManager.hpp"
#include "Application.h"
#include "mapgen.h"
#include "objects/CarMovement.h"
#include "objects/ShowOBB.h"

#include "StringOperations.hpp"

#include <algorithm>
#include <glm/geometric.hpp>
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
RaceManager::CarObject *RaceManager::AddCar(GameObject *car, bool main) {
	if (!map_manager)
		throw std::invalid_argument("init map_manager first");

	if (car->GetOBBsCount() == 0)
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
	CarObject* newCarObject = new RaceManager::CarObject(car, 0, 0);
	car_objects.push_back(newCarObject);
	car_names.insert(car->GetModelName());

	if (main) {
		main_car = car;
		std::swap(car_objects[0], car_objects[cars_placed - 1]);
	}

	return newCarObject;
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

	if (car == main_car) {
		int i = 0;
		for (; i < static_cast<int>(map_manager->GetCheckPoints()); ++i) // bro just use while
			if (map_manager->GetCheckPoint(i) == cp)
				break;

		map_manager->GetCheckPoint(i-1)->AddColorChange(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f)); // yellow -> yellow
		cp->AddColorChange(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // yellow -> red
	}

	car_obj->time = Time::lastTime;
	car_obj->checkpoint++;

	if (car_obj->checkpoint / map_manager->GetCheckPoints() >= termination_condition_value)
		EndRace();
}

double posFromProgress(double progress, double width) {
	return width * (2 * progress - 1);
}

/// <summary>
/// Start race. Has to have at least two cars and a termination condition.
/// </summary>
void RaceManager::StartRace() {
	if (!main_car)
		throw std::invalid_argument("you didn't specify the main car");
	if (car_objects.size() < 2)
		throw std::invalid_argument("add more cars");	
	
	const glm::vec4 color = glm::vec4(1, 1, 1, 1);
	const float height = .025;
	const glm::vec2 pos = glm::vec2(0, 1 - 2 * height);
	const float scale = 1.75;

	race_trackers.reserve(cars_placed);

	progress_bar = new Sprite("progress", color);
	progress_bar->rectTransform->SetWidth(.75, false)->SetHeight(height, false)->MoveTo(glm::vec3(pos, 1));

	int cp_count = map_manager->GetCheckPoints();
	checkpoint_trackers.reserve(cp_count * termination_condition_value);

	for(int lap = 0; lap < termination_condition_value; lap++)
		for (int i = 0; i < cp_count; i++) {
			Sprite* sp = new Sprite("tracker_chec", glm::vec4(1,1,1,.8));
			sp->rectTransform->SetHeight(height)->ScaleTimes(1.1)->MoveTo(glm::vec3(posFromProgress(i / static_cast<double>(cp_count * termination_condition_value) + lap / static_cast<double>(termination_condition_value), progress_bar->rectTransform->scale.x), pos.y, .85));

			checkpoint_trackers.push_back(sp);
		}

	race_trackers.reserve(cars_placed);
	for (int _ = 1; _ < cars_placed; _++) {
		Sprite* sp = new Sprite("tracker_norm", color);
		sp->rectTransform->SetHeight(height)->ScaleTimes(scale)->MoveTo(glm::vec3(pos, .5 + _ / 100.0f));

		race_trackers.push_back(sp);
	}

	Sprite* sp = new Sprite("tracker_main", color);
	sp->rectTransform->SetHeight(height)->ScaleTimes(scale)->MoveTo(glm::vec3(pos, .25));

	race_trackers.push_back(sp);

	StartAnimation();
}

void RaceManager::AfterCountdown() {
	if (countdown_number)
		delete countdown_number;
	countdown_number = nullptr;
	
	CarMovement::disabled_inputs = false;

	race_started = true;

	for (int i = 0; i < map_manager->GetCheckPoints(); ++i) {
		for (const auto &car : car_objects) {
			Collisions::addCallback(map_manager->GetCheckPoint(i), car->car, std::bind(&RaceManager::OnCheckpoint, this, std::placeholders::_1));
		}
	}

	clock = new Text("HackBold", {0.95, 0.95, 0.1}, {1, 1}, 0.3f);
	clock->SetText("Get ready!");

	velocity = new Text("HackBold", { -.95,.95,.1 }, { -1,1 }, .3);
	velocity->SetText("--");

	loop_tracker = new Text("HackBold", {0.95, 0.55, 0.1}, {1, 1}, 0.3f);
	loop_tracker->SetText("-/-");

	place = new Text("HackBold", { .95,.75,.1 }, { 1,1 }, .3);
	place->SetText("-");

	nitro_icon = new Sprite("nitro");
	nitro_icon->rectTransform->SetHeight(.08)->MoveTo({ -.925,.625,.1 })->Move({ nitro_icon->rectTransform->GetDimentions() / 2.0f });

	nitro_counter = new Text("HackBold", { -.85, .75,.1 }, { -1,1 }, .3);
	nitro_counter->SetText("-");
}

void RaceManager::CalcAvgDist()
{
	unsigned n = map_manager->GetCheckPoints();

	if(sum_x == 0)
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
	delete velocity;

	delete place;

	delete progress_bar;
	for (Sprite* t : checkpoint_trackers) delete t;
	for (Sprite* t : race_trackers) delete t;

	delete nitro_icon;
	delete nitro_counter;

	std::sort(car_objects.begin(), car_objects.end(), [](CarObject *a, CarObject *b) {
		if (a->checkpoint == b->checkpoint)
			return a->time < b->time;
		return a->checkpoint > b->checkpoint;
	});

	std::cout << "Race ended.\n";

	CarMovement::disabled_inputs = true;

	Sprite* bg = new Sprite("end_race_bg");
	bg->rectTransform->ScaleTo(glm::vec2(100.0f, 100.0f));
	bg->rectTransform->MoveTo(glm::vec3(0.0f, 0.0f, 1.0f));

	int place = 0;
	while (car_objects[place]->car != main_car)
		++place;
	++place;

	std::vector<LiveRaceObject*> lr_objects = GetLiveRace(false);
	int lr_place = 0;
	while (lr_objects[lr_place]->car != main_car)
		++lr_place;

	(new Text("SansSerif", glm::vec3(0.1f, 0.8f, 0.0f), glm::vec2(0.0f)))->SetText("The race ended")->ChangeSize(0.5);
	(new Text("SansSerif", glm::vec3(0.1f, 0.575f, 0.0f), glm::vec2(0.0f)))->SetText("...and you were " + std::to_string(place) + String::getSuffix(place) + '!')->ChangeSize(0.4);

	(new Text("SansSerif", glm::vec3(0.1f, 0.25f, 0.0f), glm::vec2(0.0f)))->SetText("Nitros used: " + std::to_string(main_car->cm->nitrosUsed))->ChangeSize(0.2);
	(new Text("SansSerif", glm::vec3(0.1f, 0.1f, 0.0f), glm::vec2(0.0f)))->SetText("Time taken: " + String::formatDouble(race_time_elapsed, 2) + 's')->ChangeSize(0.2);

	double dist = lr_objects[lr_place]->progress * sum_x * VELOCITY_DISPLAY_MULTIPLIER;
	(new Text("SansSerif", glm::vec3(0.1f, -0.05f, 0.0f), glm::vec2(0.0f)))->SetText("Distance covered: " + String::formatDouble(dist, 2) + 'm')->ChangeSize(0.2);
	(new Text("SansSerif", glm::vec3(0.1f, -0.2f, 0.0f), glm::vec2(0.0f)))->SetText("Avereage speed: " + String::formatDouble(dist / race_time_elapsed, 2) + "m/s")->ChangeSize(0.2);

	(new Text("SansSerif", glm::vec3(.1, -.7, 0), glm::vec2(0)))->SetText("Seed: " + std::to_string(map_manager->getSeed()))->ChangeSize(.125);

	(new Text("SansSerif", glm::vec3(0.1f, -0.8f, 0.0f), glm::vec2(0.0f)))->SetText("Press ENTER to continue!")->ChangeSize(0.3);

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
			+ 1 - Collisions::getL1Distance(car->car, map_manager->GetCheckPoint(car->checkpoint + 1)) / avg_cp_dist;

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
	handleNitros();

	counter_000 += Time::deltaTime;

	if (counter_000 >= .75f) {
		counter_000 -= .75f;
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
	std::string ntext = String::formatDouble(std::abs(main_car->cm->getActSpeed() * VELOCITY_DISPLAY_MULTIPLIER), 2) + " km/h";
	if(ntext != velocity->getText())
		velocity->SetText(ntext);
}

void RaceManager::handleLoops() {
	std::string ntext = (std::to_string((car_objects[0]->checkpoint / map_manager->GetCheckPoints()) + 1) + '/' + std::to_string(termination_condition_value));
	if (ntext != loop_tracker->getText())
		loop_tracker->SetText(ntext);
}

void RaceManager::handleNitros() {
	std::string ntext = std::to_string(main_car->cm->nitrosCount());
	if (ntext != nitro_counter->getText())
		nitro_counter->SetText(ntext);
}

void RaceManager::handleTracking()
{
	std::vector<LiveRaceObject*> race_data = GetLiveRace();

	bool main_found = false;
	int i = 0;
	while (i < cars_placed) {
		int c_index = i - main_found;

		if (!main_found && race_data[i]->car == main_car) {
			main_found = true;
			c_index = cars_placed - 1;

			//std::cout << "%: " << race_data[i]->progress << '\n';

			std::string ntext = std::to_string(i + 1) + String::getSuffix(i + 1);
			if (ntext != place->getText())
				place->SetText(ntext);
		}

		glm::vec3 pos = race_trackers[c_index]->rectTransform->position;
		//race_trackers[c_index]->rectTransform->MoveTo(glm::vec3(progress_bar->rectTransform->scale.x * (2 * race_data[i]->progress - 1), pos.y, pos.z));
		race_trackers[c_index]->rectTransform->MoveTo(glm::vec3(posFromProgress(race_data[i]->progress, progress_bar->rectTransform->scale.x), pos.y, pos.z));

		++i;
	}
}

std::vector<RaceManager::CarObject*>& RaceManager::getCarObjects() {
	return car_objects;
}