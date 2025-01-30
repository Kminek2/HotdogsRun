#pragma once
#include "Model.h"
#include "Transform.h"
#include "ObjectScript.h"

#include <list>
#include <vector>
#include <array>

class Commands;
class Collisions;

struct OBB {
	OBB(glm::vec3 center = glm::vec3(0.0f),
		glm::vec3 sizes = glm::vec3(1.0f),
		std::array<glm::vec3, 3> axes = {glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f)});
	glm::vec3 center;
	glm::vec3 sizes;
	std::array<glm::vec3, 3> axes;
};

class GameObject
{
private:
	Model* model;

	static std::list<GameObject*> createdGameObject;

	std::list<GameObject*>::iterator i;
	std::vector<OBB> obbs;

	static bool deletingAll;
public:
	GameObject(std::string model, glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1));
	~GameObject();

	Transform* transform;

	std::vector<ObjectScript*> objectScripts;

	void AddScript(ObjectScript* script);
	void addOBB(OBB obb);

	static void DeleteAll();
	static void UpdateAllObjectScripts();

	static void TransformTransformsToMemory();
	friend Commands;
	friend Collisions;
};

