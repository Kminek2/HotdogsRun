#include "Application.h"
#include "UniformBuffer.h"
#include "Model.h"
#include "GameObject.h"

#include "AppTime.h"
#include "Input.h"

#include <iostream>
#include <filesystem>

#include "Scene.h"
#include "scenes/LoadScene.h"

#include "LightObject.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Descriptior.h"

#include "DebugPoints.h"
#include "AudioSource3d.h"

#include "Sprite.h"
#include "Uniform.h"

namespace fs = std::filesystem;

uint16_t Application::width, Application::height;
GLFWwindow* Application::window;

Application::Application(uint16_t width, uint16_t height, GLFWwindow* window) {
	this->width = width;
	this->height = height;
	this->window = window;

	for(const auto& entry : fs::directory_iterator("models_obj_test")) {
		// std::cout << entry.path().filename().stem().string() << " : " << entry.path().string() << ", " << "textures/" + entry.path().filename().stem().string() + ".png" << '\n';
		Model::LoadModelFromFile(entry.path().filename().stem().string(), entry.path().string(), "textures/" + entry.path().filename().stem().string() + ".png", true);
	}

	Model::LoadEmptyModel();

	Model::SendBuffers();

	for (const auto& entry : fs::directory_iterator("images")) {
		// std::cout << entry.path().filename().stem().string() << " : " << entry.path().string() << ", " << "textures/" + entry.path().filename().stem().string() + ".png" << '\n';
		Sprite::LoadImageFromFile(entry.path().filename().stem().string(), entry.path().string());
	}

	Sprite::SendDataToGPU();

	for (const auto& entry : fs::directory_iterator("fonts")) {
		Font::LoadFontFromFile(entry.path().filename().stem().string(), entry.path().string(), 32U);
	}

	camera = new Camera(FRAMES_IN_FLIGHT, width, height);
	LightObject::pointLightBuffer = new UniformBuffer<PointLightBuffer>(FRAMES_IN_FLIGHT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
	LightObject::spotLightBuffer = new UniformBuffer<SpotLightBuffer>(FRAMES_IN_FLIGHT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

	Input::window = window;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Input::MouseCallback);

	Input::width = width;
	Input::height = height;

	for (int i = 0; i < LoadScene::scenes.size(); i++) {
		Scene::registerTemplate(LoadScene::scenes[i].first, LoadScene::scenes[i].second);
	}

	soundEngine = new SoundEngine();

	Input::startKeyCallback();
	Scene::loadedScene = Scene::Load(LoadScene::scenes[0].first)->Init();
}

Application::~Application() {
	delete soundEngine;
	Model::Unload();
	Transform::Unload();
	LightObject::Unload();
	DebugPoints::DeleteBuffers();
	Sprite::Unload();
	delete camera;
}

void Application::UpdateWindowSizes(uint16_t width, uint16_t height) {
	float aspWidth = width / ASPECT_WIDTH;
	float aspHeight = height / ASPECT_HEIGHT;
	
	width = std::min(aspHeight, aspWidth) * ASPECT_WIDTH;
	height = std::min(aspHeight, aspWidth) * ASPECT_HEIGHT;
	this->width = width;
	this->height = height;
	camera->UpdateCamera(width, height);

	Input::width = width;
	Input::height = height;
}

void Application::LoadScene(std::string scene)
{
	Scene::loadedScene.get()->sceneScript->UnLoad();
	Input::stopKeyCallback();

	GameObject::DeleteAll();
	Sprite::DeleteAll();
	LightObject::DeleteAll();
	PointLight::DeleteAll();
	SpotLight::DeleteAll();
	DebugPoints::ClearLines();
	Camera::main->Reload(width, height);
	AudioSource2d::DeleteAllSources2d();
	AudioSource3d::DeleteAllSources3d();

	Input::startKeyCallback();
	Scene::loadedScene = Scene::Load(scene)->Init();
}

void Application::Quit()
{
	glfwSetWindowShouldClose(window, true);
}

void Application::UpdateBuffer(uint16_t frame, Uniform *uniform)
{
	camera->UpdateCamera(width, height);
	camera->UpdateBuffer(frame);

	if (LightObject::pointLightBuffer->getSize() != PointLight::SendData(frame) && PointLight::lightNum > 0)
		uniform->UpdateDescriptorSets(*LightObject::getPointBuffer()->GetBuffer(), 2, glm::max(static_cast<uint32_t>(PointLight::lightNum * sizeof(PointLightBuffer)), static_cast<uint32_t>(1)));

	if (LightObject::spotLightBuffer->getSize() != SpotLight::SendData(frame) && SpotLight::lightNum > 0)
		uniform->UpdateDescriptorSets(*LightObject::getSpotBuffer()->GetBuffer(), 3, glm::max(static_cast<uint32_t>(SpotLight::lightNum * sizeof(SpotLightBuffer)), static_cast<uint32_t>(1)));
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
		//std::cout << "fps: " << times << '\n';

		frameTimes.clear();
	}
	Scene::loadedScene.get()->sceneScript->EarlyUpdate();
	GameObject::EarlyUpdateAllObjectScripts();
	Scene::loadedScene.get()->sceneScript->Update();
	GameObject::UpdateAllObjectScripts();
	Scene::loadedScene.get()->sceneScript->LateUpdate();
	GameObject::LateUpdateAllObjectScripts();

	soundEngine->UpdatePos();
	AudioSource3d::UpdateAllPosition();
	Input::mouseOff = Input::mousePos - Input::lastPos;
	Transform::ClearMemory();
	GameObject::TransformTransformsToMemory();
	Sprite::UpdateBuffer();
	Transform::TransformToMemory();
}
