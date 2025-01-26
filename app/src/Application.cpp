#include "Application.h"
#include "UniformBuffer.h"
#include "Model.h"
#include "GameObject.h"

#include "AppTime.h"
#include "Input.h"

#include <iostream>

#include "Scene.h"
#include "scenes/LoadScene.h"

uint16_t Application::width, Application::height;

Application::Application(uint16_t width, uint16_t height, GLFWwindow* window) {
	this->width = width;
	this->height = height;
	Model::LoadModelFromFile("test", "3x3x3.vox");
	Model::LoadModelFromFile("racing_car", "racing_car.vox");

	Model::SendBuffers();

	camera = new Camera(FRAMES_IN_FLIGHT, width, height);

	Input::window = window;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Input::MouseCallback);

	Input::width = width;
	Input::height = height;

	for (int i = 0; i < LoadScene::scenes.size(); i++) {
		Scene::registerTemplate(LoadScene::scenes[i].first, LoadScene::scenes[i].second);
	}

	Input::startKeyCallback();
	Scene::loadedScene = Scene::Load(LoadScene::scenes[0].first)->Init();
}

Application::~Application() {
	Model::Unload();
	delete camera;
}

void Application::UpdateCamera(uint16_t width, uint16_t height) {
	this->width = width;
	this->height = height;
	camera->UpdateCamera(width, height);
}

void Application::LoadScene(std::string scene)
{
	Scene::loadedScene.get()->sceneScript->UnLoad();
	Input::stopKeyCallback();

	GameObject::DeleteAll();
	Camera::main->Reload(width, height);

	Input::startKeyCallback();
	Scene::loadedScene = Scene::Load(scene)->Init();
}

void Application::UpdateBuffer(uint16_t frame)
{
	camera->UpdateCamera(width, height);
	camera->UpdateBuffer(frame);
}

std::list <float> frameTimes;

void Application::Update() {
	Time::UpdateTime();
	frameTimes.push_back(1 / Time::deltaTime);

	if (frameTimes.size() >= 500) {
		std::list <float>::iterator it = frameTimes.begin();

		float times = 0;

		while (it != frameTimes.end())
		{
			times += *it;
			it = std::next(it);
		}

		times /= frameTimes.size();
		std::cout << "fps: " << times << '\n';

		frameTimes.clear();
	}

	Scene::loadedScene.get()->sceneScript->Update();
	GameObject::UpdateAllObjectScripts();

	Input::mouseOffX = Input::mousePosX - Input::lastX;
	Input::mouseOffY = Input::mousePosY - Input::lastY;
	GameObject::TransformTransformsToMemory();
}