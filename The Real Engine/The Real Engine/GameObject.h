#pragma once
#include "Model.h"
#include "Transform.h"

#include <list>

class Commands;

class GameObject
{
private:
	Model* model;

	static std::list<GameObject*> createdGameObject;

	std::list<GameObject*>::iterator i;
public:
	GameObject(std::string model, glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1));
	~GameObject();

	Transform* transform;

	static void DeleteAll();

	static void TransformTransformsToMemory();
	friend Commands;
};

