#include "Scene.h"
#include "scenes/TutorialScene.hpp"

#include "objects/CarMovement.h"
#include "objects/WheelsScript.h"
#include "objects/CarInputs.h"
#include "objects/SmoothCamera.hpp"

#include "_rand.hpp"
#include "StringOperations.hpp"

std::shared_ptr<Scene> TutorialScene::Init() {
	Scene* scene = new Scene(this);

	_rand rand(String::getHash("TUTORIAL"));

	const int seg_len = 5;

	const auto createVector = [seg_len](int j) {
		std::vector<glm::vec2> points;
		points.reserve(seg_len);
		for (int i = 0; i < seg_len; i++) points.emplace_back(((i % 2) == 1 ? ((i % 4) == 1 ? -1 : 1) : 0), j * seg_len + i);
		return points;
	};

	__road roadmap_asphalt = createRoadMap("Asfalt");
	__road roadmap_ice = createRoadMap("Lod");
	__road roadmap_gravel = createRoadMap("Zwir");

	const float MAP_TILE_SIZE = 12.7;
	const float MAP_TILE_SCALE = 2;

	const auto createRoads = [MAP_TILE_SIZE, MAP_TILE_SCALE, createVector](__road rm, int j, int st) {
		std::vector<glm::vec2> vec = createVector(j);

		for (int i = 0; i < vec.size(); i++) {
			direction in, out;

			if (i == 0 || vec[i - 1].x == vec[i].x) in = W;
			else if (vec[i - 1].x < vec[i].x) in = SW;
			else in = NW;

			if (i == vec.size() - 1 || vec[i].x == vec[i + 1].x) out = E;
			else if (vec[i].x < vec[i + 1].x) out = NE;
			else out = SE;

			ObjectSchema* r = rm.find({ in,out }) == rm.end() ? rm.at({ out,in }) : rm.at({ in,out });
			r = new ObjectSchema(*r);

			r->scale *= MAP_TILE_SCALE;
			r->surface_type = st;

			(new GameObject(r))->AddDefaultOBB(glm::vec3(0), true)->transform->MoveTo(glm::vec3(vec[i], -.005) * MAP_TILE_SIZE * MAP_TILE_SCALE);
		}
	};

	createRoads(roadmap_asphalt, 0, 1);
	createRoads(roadmap_ice, 1, 3);
	createRoads(roadmap_gravel, 2, 2);

	GameObject* car = new GameObject("racing_car", glm::vec3(0, 0, .2), glm::vec3(0, 0, -90));

	car->AddDefaultOBB();

	CarMovement* cmv = new CarMovement(1.0f, 1.0f, 600.0f, -100.0f, 100.0f, 20.0f, 0.1f, false, 0.05f);
	car->AddScript(cmv);
	car->AddScript(new WheelsScript(*cmv, "", 0.9f, 0.9f, 0.0f, 2.2f));
	car->AddScript(new CarInputs(*cmv, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_CONTROL));

	view_scripts[0] = new CameraLockScript(Perspective, glm::vec3(-50, 0, 0), -90.0f, 45.0f, false, false, GLFW_KEY_Z, GLFW_KEY_C);
	view_scripts[1] = new CameraLockScript(Perspective, glm::vec3(1.7f, 0.0f, 1.5f), 0.0f, 90.0f, true, false, GLFW_KEY_Z, GLFW_KEY_C);

	car->AddScript(view_scripts[0]);
	car->AddScript((new SmoothCamera(glm::vec3(10, 0, 3), 5.0f, Perspective))->SetDynamicFov(true, 45.0f, 50.0f));
	car->AddScript(view_scripts[1]);

	view_scripts[0]->disabled2 = true;
	view_scripts[1]->disabled2 = true;

	(new GameObject("szlabanPodstawa", glm::vec3(0, -.5 * MAP_TILE_SIZE * MAP_TILE_SCALE, 0)))->AddDefaultOBB()->transform->ScaleTimes(MAP_TILE_SCALE);
	(new GameObject("szlabanRurka", glm::vec3(0, -.5 * MAP_TILE_SIZE * MAP_TILE_SCALE, 1)))->AddDefaultOBB()->transform->ScaleTimes(MAP_TILE_SCALE);
	
	(new GameObject("szlabanPodstawa", glm::vec3(0, (3 * seg_len - .5) * MAP_TILE_SIZE * MAP_TILE_SCALE, 0)))->AddDefaultOBB()->transform->ScaleTimes(MAP_TILE_SCALE);
	(new GameObject("szlabanRurka", glm::vec3(0, (3 * seg_len - .5) * MAP_TILE_SIZE * MAP_TILE_SCALE, 1)))->AddDefaultOBB()->transform->ScaleTimes(MAP_TILE_SCALE);

	(new GameObject("BaseCube", glm::vec3(0, 0, -0.23f), glm::vec3(0), glm::vec3(1000, 1000, 0)))->AddColorChange(glm::vec3(1), glm::vec3(0.1f, 0.5f, 0.1f));

	main_text_bg = new Sprite("settings_bg", glm::vec4(1, 1, 1, .5));
	main_text_bg->rectTransform->SetWidth(1)->SetHeight(.4, false)->MoveTo(glm::vec3(0, .8, 1));

	main_text.push_back(new Text("SansSerif", glm::vec3(0, .925, .9), glm::vec2(0), .2));
	main_text.push_back(new Text("SansSerif", glm::vec3(0, .825, .9), glm::vec2(0), .2));
	main_text.push_back(new Text("SansSerif", glm::vec3(0, .725, .9), glm::vec2(0), .2));
	main_text.push_back(new Text("SansSerif", glm::vec3(0, .625, .9), glm::vec2(0), .2));

	checkpoint = (new GameObject("checkpoint", glm::vec3(0, 2 * seg_len, 0) * MAP_TILE_SIZE * MAP_TILE_SCALE))->AddDefaultOBB(glm::vec3(1));
	checkpoint->transform->ScaleTimes(2);
	checkpoint->surface_type = NEVER_COLLIDE;

	Collisions::addCallback(checkpoint, car, [this](Collisions::CollisionData* cd) {
		checkpoint->AddColorChange(glm::vec3(1, 1, 0), glm::vec3(1, 0, 0));
		cp_color_timer = 0.0f;
	});

	const float forest_dist = 100.0f;
	int trees_count = 125;
	const std::vector<std::string> trees_models = { "krzak","malyKrzak","drzewo" };
	//const std::vector<std::string> trees_models = { "brzoza","drzewo","jodla","krzak","maleDrzewo","malyKrzak" };
	while (trees_count --> 0) {
		glm::vec2 pos = glm::vec2(rand.coin_toss() ? rand.random(-forest_dist, -20.0f) : rand.random(20.0f, forest_dist), rand.random(-forest_dist, 3 * seg_len * MAP_TILE_SIZE * MAP_TILE_SCALE + forest_dist));

		for (int i = 0; i < rand.random(4, 8); i++) {
			GameObject* o = (new GameObject(rand.choice(trees_models), glm::vec3(pos, 0) + glm::vec3(rand.random(-5.0f, 5.0f), rand.random(-5.0f, 5.0f), 0)))->AddDefaultOBB();

			o->transform->ScaleTimes(rand.random(0.75 * MAP_TILE_SCALE, 1.25 * MAP_TILE_SCALE));
			o->transform->RotateTo(glm::vec3(0, 0, rand.random(0.0f, 360.0f)));
		}
	}

	const auto createObstacles = [MAP_TILE_SCALE, MAP_TILE_SIZE](int x) { (new GameObject("pacholki", glm::vec3(0, x, 0) * MAP_TILE_SIZE * MAP_TILE_SCALE))->AddDefaultOBB()->transform->ScaleTimes(MAP_TILE_SCALE); };
	for (int i = 1; i < 3; i++) createObstacles(i * seg_len + 2);

	return std::shared_ptr<Scene>(scene);
}

std::vector<std::vector<std::string>> tutorial_texts = {
	{ "Welcome to the race!", "Let's learn the basics!", "Use WASD to drive.", "Press SPACE to handbrake! [ENTER for next]" },
	{ "Want more speed?", "Hold LCTRL for nitro!", "Use it wisely!", "It runs out fast!" },
	{ "Switch your view!", "Press C to change camera.", "Try different angles!", "Find your favorite!" },
	{ "Surfaces matter!", "Asphalt gives best grip.", "Gravel is slower.", "Ice? Slippery fun!" },
	{ "Stay on track!", "Going off-road is bad.", "You'll slow down a lot!", "Stick to the path!" },
	{ "Watch out!", "Obstacles can stop you!", "Avoid them to stay fast!", "React quickly!" },
	{ "But not all is bad!", "Grab powerups!", "They help you win!", "Use them wisely!" },
	{ "Checkpoints ahead!", "Drive through each one.", "It turns red when passed!", "Don't miss any!" },
	{ "That's all, racer!", "Now hit the track!", "Show them your skills!", "Good luck! [ESC to exit]" }
};

void TutorialScene::Update() {
	if (Input::getKeyClicked(GLFW_KEY_C)) {
		camera_view = glm::normalize(camera_view + 1, 3);

		SmoothCamera::disabled2 = false;
		view_scripts[0]->disabled2 = false;
		view_scripts[1]->disabled2 = false;

		if (camera_view != 0)    SmoothCamera::disabled2 = true;
		if (camera_view != 1) view_scripts[0]->disabled2 = true;
		if (camera_view != 2) view_scripts[1]->disabled2 = true;
	}

	if (text_id == -1) setText(tutorial_texts[++text_id]);

	if (Input::getKeyClicked(GLFW_KEY_ENTER)) {
		setText(tutorial_texts[std::min(++text_id, static_cast<int>(tutorial_texts.size() - 1))]);
	}

	if (cp_color_timer < 5.0f) {
		cp_color_timer += Time::deltaTime;
		if (cp_color_timer >= 5.0f)
			checkpoint->AddColorChange(glm::vec3(1, 1, 0), glm::vec3(1, 1, 0));
	}

	if (Input::getKeyClicked(GLFW_KEY_ESCAPE)) {
		Application::LoadScene("main_menu");
		return;
	}
}

void TutorialScene::UnLoad() {
	delete main_text_bg;
	for (Text* t : main_text) delete t;
}

void TutorialScene::setText(std::vector<std::string> txt)
{
	for (int i = 0; i < main_text.size(); i++) 
		main_text[i]->SetText(i < txt.size() ? txt[i] : " ");
}