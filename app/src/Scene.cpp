#include "Scene.h"

std::unordered_map<std::string, SceeneScript*> Scene::templates;

std::shared_ptr<Scene> Scene::loadedScene;

Scene::Scene(SceeneScript* sceneScript)
{
	this->sceneScript = sceneScript;
}
