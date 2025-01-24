#pragma once
#include "Model.h"
#include "Transform.h"
#include "ObjectScript.h"

#include <list>
#include <vector>

class Commands;

class GameObject
{
private:
	Model* model;

	static std::list<GameObject*> createdGameObject;

	std::list<GameObject*>::iterator i;

	static bool deletingAll;
public:
	GameObject(std::string model, glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1));
	~GameObject();

	Transform* transform;

	std::vector<ObjectScript*> objectScripts;

	void AddScript(ObjectScript* script);

	static void DeleteAll();
	static void UpdateAllObjectScripts();

	static void TransformTransformsToMemory();
	friend Commands;
};

