#include "Collisions.h"
#include "GameObject.h"
#include <glm/fwd.hpp>

bool Collisions::checkOBBCollision(const OBB& a, const OBB& b) {
	glm::mat3 rotation, absRotation;
	glm::vec3 distance = b.center - a.center;

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			rotation[i][j] = glm::dot(a.axes[i], b.axes[j]);

	distance = glm::vec3(glm::dot(distance, a.axes[0]), glm::dot(distance, a.axes[1]), glm::dot(distance, a.axes[2]));

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			absRotation[i][j] = std::abs(rotation[i][j]) + 1e-6f;

	for (int i = 0; i < 3; ++i) {
		float ra = a.sizes[i];
		float rb = b.sizes[0]*absRotation[i][0] + b.sizes[1]*absRotation[i][1] + b.sizes[2]*absRotation[i][2];
		if (std::abs(distance[i]) > ra + rb) 
			return false;
	}

	for (int i = 0; i < 3; ++i) {
		float ra = a.sizes[0]*absRotation[0][i] + a.sizes[1]*absRotation[1][i] + a.sizes[2]*absRotation[2][i];
		float rb = b.sizes[i];
		if (std::abs(distance[0]*rotation[0][i] + distance[1]*rotation[1][i] + distance[2]*rotation[2][i]) > ra + rb) 
			return false;
	}

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j) {
			float ra = a.sizes[(i+1)%3]*absRotation[(i+2)%3][j] + a.sizes[(i+2)%3]*absRotation[(i+1)%3][j];
			float rb = b.sizes[(j+1)%3]*absRotation[i][(j+2)%3] + b.sizes[(j+2)%3]*absRotation[i][(j+1)%3];
			if (std::abs(distance[(i+1)%3]*rotation[(i+2)%3][j] - distance[(i+2)%3]*rotation[(i+1)%3][j]) > ra + rb)
				return false;
		}

	return true;
}

bool Collisions::checkCollision(const GameObject& obj1, const GameObject& obj2, bool callback) {
	if (obj1.obbs.size() == 0 || obj2.obbs.size() == 0)
		return false;

	float obj1S;
	if (obj1.model->GetName() != "")
		obj1S = obj1.GetModelMaxDistVert()[0].x * obj1.GetModelMaxDistVert()[0].y + obj1.GetModelMaxDistVert()[1].x * obj1.GetModelMaxDistVert()[1].y + obj1.GetModelMaxDistVert()[2].x * obj1.GetModelMaxDistVert()[2].y;
	else
		obj1S = obj1.obbs[0].sizes.x * obj1.obbs[0].sizes.x + obj1.obbs[0].sizes.y * obj1.obbs[0].sizes.y + obj1.obbs[0].sizes.z * obj1.obbs[0].sizes.z;
	float obj2S;
	if (obj2.model->GetName() != "")
		obj2S = obj2.GetModelMaxDistVert()[0].x * obj2.GetModelMaxDistVert()[0].y + obj2.GetModelMaxDistVert()[1].x * obj2.GetModelMaxDistVert()[1].y + obj2.GetModelMaxDistVert()[2].x * obj2.GetModelMaxDistVert()[2].y;
	else
		obj2S = obj2.obbs[0].sizes.x * obj2.obbs[0].sizes.x + obj2.obbs[0].sizes.y * obj2.obbs[0].sizes.y + obj2.obbs[0].sizes.z * obj2.obbs[0].sizes.z;

	glm::vec3 posDif = (obj1.transform->position - obj2.transform->position);
	if ((obj1S * std::max(obj1.transform->scale.x, std::max(obj1.transform->scale.y, obj1.transform->scale.z)) + obj2S * std::max(obj2.transform->scale.x, std::max(obj2.transform->scale.y, obj2.transform->scale.z))) > (posDif.x * posDif.x + posDif.y * posDif.y + posDif.z * posDif.z) * 4.0f)
		return false;
		

	bool coll = false;

	for (int i = 0; i < obj1.obbs.size(); ++i) {
		OBB absOBB1 = getAbsOBB(obj1.obbs[i], obj1);

		for (int j = 0; j < obj2.obbs.size(); ++j) {
			OBB absOBB2 = getAbsOBB(obj2.obbs[j], obj2);

			if (checkOBBCollision(absOBB1, absOBB2)) {
				coll = true;
				break;
			}
		}

		if (coll) break;
	}

	if (!coll || !callback) return coll;

	const std::pair<GameObject*, GameObject*> colliders = { std::min(const_cast<GameObject*>(&obj1), const_cast<GameObject*>(&obj2)), std::max(const_cast<GameObject*>(&obj1), const_cast<GameObject*>(&obj2)) };

	if (callbacks.find(colliders) != callbacks.end()) {
		CollisionData* collision_data = new CollisionData(const_cast<GameObject*>(&obj1), const_cast<GameObject*>(&obj2));
		for (const auto& cb : callbacks[colliders])
			cb(collision_data);
	}

	return true;
}

OBB Collisions::getAbsOBB(const OBB& obb, const GameObject& obj) {
	OBB absOBB;
	glm::mat4 rotationMatrix = getRotationMatrix(obj.transform->rotation, obj.transform->scale/abs(obj.transform->scale));
	glm::vec3 absPosition = glm::vec3(obj.transform->position.x, obj.transform->position.y, obj.transform->position.z);

	absOBB.center = obj.transform->getModelMatrix() * glm::vec4(obb.center, 1);
	
	for (int i = 0; i < 3; ++i) {
		absOBB.axes[i] = rotationMatrix * glm::vec4(obb.axes[i], 1.0f);
	}

	absOBB.sizes = obb.sizes;
	absOBB.sizes *= abs(obj.transform->scale);

	return absOBB;
}

glm::mat4 Collisions::getRotationMatrix(glm::vec3 rot, glm::vec3 scale) {
	glm::vec3 v = glm::radians(rot);
	glm::mat4 model_transform = glm::mat4(1);

	model_transform = glm::rotate(model_transform, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model_transform = glm::rotate(model_transform, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model_transform = glm::rotate(model_transform, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	
	model_transform = glm::scale(model_transform, scale);

	return model_transform;
}

std::map<std::pair<GameObject*, GameObject*>, std::vector<std::function<void(Collisions::CollisionData*)>>> Collisions::callbacks;
void Collisions::addCallback(GameObject* a, GameObject* b, std::function<void(Collisions::CollisionData*)> callback) {
	callbacks[{std::min(a, b), std::max(a, b)}].push_back(callback);
}

float Collisions::getL1Distance(glm::vec3 a, glm::vec3 b) { return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z); }
float Collisions::getL1Distance(GameObject* a, GameObject* b) { return getL1Distance(a->transform->position, b->transform->position); }