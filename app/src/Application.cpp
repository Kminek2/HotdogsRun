#include "Application.h"
#include "UniformBuffer.h"
#include "Model.h"
#include "GameObject.h"

#include "AppTime.h"
#include "Input.h"

#include <iostream>
#include <filesystem>
#include <thread>

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
uint16_t Application::unscaledWidth, Application::unscaledHeight;
GLFWwindow* Application::window;

Application::Application(uint16_t width, uint16_t height, GLFWwindow* window) {
	errorLastFrame = false;

	this->unscaledHeight = height;
	this->unscaledWidth = width;
	this->width = width;
	this->height = height;
	this->window = window;

	for (int i = 0; i < LoadScene::preLoadModels.size(); i++)
	{
		Model::LoadModelFromFile(LoadScene::preLoadModels[i],
			"models_obj/" + LoadScene::preLoadModels[i] + ".obj", 
			"textures/" + LoadScene::preLoadModels[i] + ".png", true, LoadScene::preLoadModels[i] != "Sprite");
	}
	
	for (int i = 0; i < LoadScene::preLoadSprites.size(); i++)
	{
		Sprite::LoadImageFromFile(LoadScene::preLoadModels[i], 
			"textures/" + LoadScene::preLoadModels[i] + ".png");
	}

	for (const auto& entry : fs::directory_iterator("models_obj")) {
		entries.push_back(entry);
	}

	Model::LoadEmptyModel(); // responsible for GameObject() and GameObject("")

	Model::SendBuffers();

	loadedModel = 0;
	loadedAll = false;

	modelLoading = std::thread([this]() {
		for (const auto& entry : entries) {
			std::string modelName = entry.path().filename().stem().string();

			if (!Model::LoadedAModel(modelName)) {
				Model::LoadModelFromFile(
					modelName,
					entry.path().string(),
					"textures/" + modelName + ".png",
					true,
					entry.path().string() != "Sprite"
				);
			}
			loadedModel++;
		}
		});

	for (const auto& entry : fs::directory_iterator("images")) {
		Sprite::LoadImageFromFile(
			entry.path().filename().stem().string(),
			entry.path().string()
		);
	}

	Sprite::SendDataToGPU();

	for (const auto& entry : fs::directory_iterator("fonts")) {
		Font::LoadFontFromFile(entry.path().filename().stem().string(), entry.path().string(), 32U, 20U);
	}

	LightObject::Init();

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
	unscaledWidth = width;
	unscaledHeight = height;
	float aspWidth = width / ASPECT_WIDTH;
	float aspHeight = height / ASPECT_HEIGHT;
	
	width = std::min(aspHeight, aspWidth) * ASPECT_WIDTH;
	height = std::min(aspHeight, aspWidth) * ASPECT_HEIGHT;
	this->width = width;
	this->height = height;
	camera->UpdateCamera(unscaledWidth, unscaledHeight);

	Input::offset = glm::vec2((unscaledWidth - width) / 2.0f, (unscaledHeight - height) / 2.0f);
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
	camera->UpdateCamera(unscaledWidth, unscaledHeight);
	camera->UpdateBuffer(frame, cubeMapUniform);

	if (LightObject::pointLightBuffer->getSize() != PointLight::SendData(frame) && PointLight::lightNum > 0)
		uniform->UpdateDescriptorSets(*LightObject::getPointBuffer()->GetBuffer(), 2, glm::max(static_cast<uint32_t>(PointLight::lightNum * sizeof(PointLightBuffer)), static_cast<uint32_t>(1)));

	if (LightObject::spotLightBuffer->getSize() != SpotLight::SendData(frame) && SpotLight::lightNum > 0)
		uniform->UpdateDescriptorSets(*LightObject::getSpotBuffer()->GetBuffer(), 3, glm::max(static_cast<uint32_t>(SpotLight::lightNum * sizeof(SpotLightBuffer)), static_cast<uint32_t>(1)));

	if (GameObject::allColorChanges->getSize() != GameObject::SendColorData(frame) && GameObject::changeColor.size() > 0)
		uniform->UpdateDescriptorSets(*GameObject::allColorChanges->GetBuffer(), 4, glm::max(GameObject::allColorChanges->getSize(), static_cast<VkDeviceSize>(1)));
}

std::list <float> frameTimes;

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

void Application::Update() {
	while (!threadPool.isEmpty())
		continue;

	if (!loadedAll && loadedModel == entries.size()) {
		modelLoading.join();
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
		std::cout << "fps: " << times << '\n';

		frameTimes.clear();
	}

	try {
		threadPool.enqueue(std::bind(&SceeneScript::EarlyUpdate, Scene::loadedScene.get()->sceneScript));
		GameObject::EarlyUpdateAllObjectScripts(threadPool);
		while (!threadPool.isEmpty())
			continue;
		threadPool.enqueue(std::bind(&SceeneScript::Update, Scene::loadedScene.get()->sceneScript));
		GameObject::UpdateAllObjectScripts(threadPool);
		while (!threadPool.isEmpty())
			continue;
		threadPool.enqueue(std::bind(&SceeneScript::LateUpdate, Scene::loadedScene.get()->sceneScript));
		GameObject::LateUpdateAllObjectScripts(threadPool);
		while (!threadPool.isEmpty())
			continue;
		threadPool.enqueue(LightObject::UpdateShadows); // TODO: make it all lights

		soundEngine->UpdatePos();

		threadPool.enqueue(AudioSource3d::UpdateAllPosition);
		while (!threadPool.isEmpty())
			continue;

		errorLastFrame = false;
	}
	catch (std::exception e)
	{
		if (errorLastFrame)
		{
#ifdef _WIN32
			MessageBox(
				NULL,
				"Application failed and will be closed",
				"Error",
				MB_ICONERROR | MB_OK| MB_DEFBUTTON1
			);
#endif // _WIN32

			throw std::runtime_error("Application failed");
		}
		std::cout << "Application update error: " << e.what() << ". Trying to fix it...\n";
		threadPool.StopAll();

		errorLastFrame = true;
	}
	Transform::ClearMemory();
	GameObject::TransformTransformsToMemory();
	Sprite::UpdateBuffer();
	Transform::TransformToMemory();
	Input::mouseOff = Input::mousePos - Input::lastPos;
}
