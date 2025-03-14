#pragma once
#include "Model.h"
#include "Transform.h"
#include "ObjectScript.h"
#include "ObjectSchema.hpp"

#include <list>
#include <vector>
#include <array>

#include "ColorChangeBuffer.h"
class Commands;
class Collisions;
class ShowOBB;
class CarMovement;

// used in collisions somewhere down the line
constexpr int ALWAYS_COLLIDE = -1;
constexpr int NEVER_COLLIDE = -2;

struct OBB {
	glm::vec3 center;
	glm::vec3 sizes;
	std::array<glm::vec3, 3> axes;

	static constexpr std::array<glm::vec3, 3> DefaultAxes = {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	};

	OBB(glm::vec3 _center = glm::vec3(0.0f),
		glm::vec3 _sizes = glm::vec3(1.0f),
		std::array<glm::vec3, 3> _axes = DefaultAxes)
		: center(_center), sizes(_sizes), axes(_axes) {}
};

class GameObject
{
private:
	Model* model;

	static std::list<GameObject*> createdGameObject;

	std::list<GameObject*>::iterator i;
	std::vector<OBB> obbs;

	static bool deletingAll;

	std::vector<ColorChangeBuffer> changeColor;
public:
	GameObject(std::string model = "", glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1), int surface_type = -1);
	GameObject(ObjectSchema* schema, glm::vec3 position = glm::vec3(0)) : GameObject(schema->model, position, schema->rotation, schema->scale, schema->surface_type) {};
	~GameObject();

	Transform* transform;

	std::vector<ObjectScript*> objectScripts;
	int surface_type;

	GameObject* AddScript(ObjectScript* script);
	std::vector<ObjectScript*>& GetObjectScripts();
	std::string GetModelName();
	std::array<glm::vec2, 3> GetModelMaxDistVert();
	GameObject* addOBB(OBB obb);
	GameObject* AddDefaultOBB(glm::vec3 offset = {0.0f, 0.0f, 0.0f}, bool two_dim = false);

	int GetOBBsCount();

	void AddColorChange(glm::vec3 from, glm::vec3 to);
	GameObject* ChangeModel(std::string model = "");

	static void DeleteAll();
	static void EarlyUpdateAllObjectScripts();
	static void UpdateAllObjectScripts();
	static void LateUpdateAllObjectScripts();

	static void TransformTransformsToMemory();
	friend Commands;
	friend Collisions;
	friend ShowOBB;
	friend CarMovement;
protected:
	GameObject(int, std::string model, glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1), int surface_type = -1);
};

