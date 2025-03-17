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

	for (int i = 0; i < LoadScene::preLoadModels.size(); i++)
	{
		Model::LoadModelFromFile(LoadScene::preLoadModels[i],"models_obj_test/" + LoadScene::preLoadModels[i] + ".obj", "textures/" + LoadScene::preLoadModels[i] + ".png", true);
	}

	for (const auto& entry : fs::directory_iterator("models_obj_test")) {
		entries.push_back(entry);
	}


	Model::LoadEmptyModel();

	Model::SendBuffers();

	loadedModel = 0;
	loadedAll = false;

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
	glfwSetCursorPosCallback(window, Input::MouseCallback);

	Input::width = width;
	Input::height = height;

	for (int i = 0; i < LoadScene::scenes.size(); i++) {
		Scene::registerTemplate(LoadScene::scenes[i].first, LoadScene::scenes[i].second);
	}

	soundEngine = new SoundEngine();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	Input::startKeyCallback();
	Scene::loadedScene = Scene::Load(LoadScene::scenes[0].first)->Init();

	GameObject::TransformTransformsToMemory();
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

void Application::SetCursor(bool enabled)
{
	if(enabled)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::LoadScene(std::string scene)
{
	Scene::loadedScene.get()->sceneScript->UnLoad();
	Input::stopKeyCallback();
	SetCursor(false);

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

void Application::UpdateBuffer(uint16_t frame, Uniform *uniform, Uniform* cubeMapUniform)
{
	camera->UpdateCamera(width, height);
	camera->UpdateBuffer(frame, cubeMapUniform);

	if (LightObject::pointLightBuffer->getSize() != PointLight::SendData(frame) && PointLight::lightNum > 0)
		uniform->UpdateDescriptorSets(*LightObject::getPointBuffer()->GetBuffer(), 2, glm::max(static_cast<uint32_t>(PointLight::lightNum * sizeof(PointLightBuffer)), static_cast<uint32_t>(1)));

	if (LightObject::spotLightBuffer->getSize() != SpotLight::SendData(frame) && SpotLight::lightNum > 0)
		uniform->UpdateDescriptorSets(*LightObject::getSpotBuffer()->GetBuffer(), 3, glm::max(static_cast<uint32_t>(SpotLight::lightNum * sizeof(SpotLightBuffer)), static_cast<uint32_t>(1)));

	if (GameObject::allColorChanges->getSize() != GameObject::SendColorData(frame) && GameObject::changeColor.size() > 0)
		uniform->UpdateDescriptorSets(*GameObject::allColorChanges->GetBuffer(), 4, glm::max(static_cast<uint32_t>(GameObject::allColorChanges->getSize()), static_cast<uint32_t>(1)));
}

std::list <float> frameTimes;

void Application::Update() {
	if (loadedModel < entries.size()) {
		const auto& entry = entries[loadedModel];
		std::string modelName = entry.path().filename().stem().string();

		if (!Model::LoadedAModel(modelName)) {
			Model::LoadModelFromFile(
				modelName,
				entry.path().string(),
				"textures/" + modelName + ".png",
				true
			);
		}
		loadedModel++;
	}
	if (loadedModel == entries.size() && !loadedAll) {
		Model::SendBuffers();
		vkDeviceWaitIdle(Device::getDevice());
		loadedAll = true;
	}
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
	Transform::ClearMemory();
	GameObject::TransformTransformsToMemory();
	Sprite::UpdateBuffer();
	Transform::TransformToMemory();
	Input::mouseOff = Input::mousePos - Input::lastPos;
}
