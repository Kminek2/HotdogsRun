#pragma once
#include "Model.h"
#include "Transform.h"
#include "ObjectScript.h"
#include "ObjectSchema.hpp"

#include <list>
#include <vector>
#include <array>
#include <set>

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

struct ColorChangeStruct {
	alignas(16) glm::vec3 from;
	alignas(16) glm::vec3 to;
};

struct ThisColorChanges {
	uint32_t amount;
	uint32_t index;

	static VkVertexInputBindingDescription GetBindingDescription(uint16_t binding = 3)
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = binding;
		bindingDescription.stride = sizeof(ThisColorChanges);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
		return bindingDescription;
	}

	static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions(uint16_t binding = 3, uint16_t location = 8) {
		std::vector<VkVertexInputAttributeDescription> attributeDescription(2);

		attributeDescription[0].binding = binding;
		attributeDescription[0].location = location;
		attributeDescription[0].format = VK_FORMAT_R32_UINT;
		attributeDescription[0].offset = 0;

		attributeDescription[1].binding = binding;
		attributeDescription[1].location = location + 1;
		attributeDescription[1].format = VK_FORMAT_R32_UINT;
		attributeDescription[1].offset = sizeof(uint32_t);

		return attributeDescription;
	}
};

class GameObject
{
private:
	Model* model;

	static std::list<GameObject*> createdGameObject;
	std::pair<int,int> act_chunk;

	std::list<GameObject*>::iterator i;
	std::vector<OBB> obbs;

	std::list<ColorChangeStruct>::iterator colorChangesIndex;
	uint32_t amountOfColorChanges;

	static bool deletingAll;

	static std::list<ColorChangeStruct> changeColor;

	static Buffer<ThisColorChanges>* colorChangesPrObject;
	static UniformBuffer<ColorChangeStruct>* allColorChanges;

	static uint32_t SendColorData(uint32_t frame);

	void EarlyUpdate(ThreadPool& threadPool);
	void Update(ThreadPool& threadPool);
	void LateUpdate(ThreadPool& threadPool);
	
	void UpdateChunk(std::pair<int,int> old_chunk);
public:
	GameObject(std::string model = "", glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1), int surface_type = -1);
	GameObject(ObjectSchema* schema, glm::vec3 position = glm::vec3(0)) : GameObject(schema->model, position, schema->rotation, schema->scale, schema->surface_type) {};
	~GameObject();
	static std::map<std::pair<int,int>, std::set<GameObject*>> chunks;

	Transform* transform;
	
	std::vector<ObjectScript*> objectScripts;
	CarMovement* cm = nullptr;

	std::pair<int,int> getChunk();

	int surface_type;

	GameObject* AddScript(ObjectScript* script);
	std::vector<ObjectScript*>& GetObjectScripts();
	std::string GetModelName();
	std::vector<OBB> GetOBBs();
	std::array<glm::vec2, 3> GetModelMaxDistVert() const;
	GameObject* addOBB(OBB obb);
	GameObject* AddDefaultOBB(glm::vec3 offset = {0.0f, 0.0f, 0.0f}, bool two_dim = false);

	int GetOBBsCount();

	void AddColorChange(glm::vec3 from, glm::vec3 to);
	GameObject* ChangeModel(std::string model = "");

	static void DeleteAll();
	static void EarlyUpdateAllObjectScripts(ThreadPool& threadPool);
	static void UpdateAllObjectScripts(ThreadPool& threadPool);
	static void LateUpdateAllObjectScripts(ThreadPool& threadPool);

	static void TransformTransformsToMemory();

	static std::list<GameObject*>& getAllGameObjects();
	friend Commands;
	friend Collisions;
	friend ShowOBB;
	friend CarMovement;
	friend class Application;
	friend class RenderPass;

	friend class StraightKingBot;
	friend class MedBot;
	friend class AntiPersonelBot;
	friend class BadBreakingBot;
protected:
	GameObject(int, std::string model, glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1), int surface_type = -1);
};

