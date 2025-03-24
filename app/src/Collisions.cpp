#include "Collisions.h"
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
	glm::vec3 pos_diff = obj1.transform->position - obj2.transform->position;
	float pos_diff_dist = pos_diff.x*pos_diff.x+pos_diff.y*pos_diff.y+pos_diff.z*pos_diff.z;
	std::array<glm::vec2, 3> ver1 = obj1.model->GetMaxDistVert();
	std::array<glm::vec2, 3> ver2 = obj2.model->GetMaxDistVert();
	float obj1_max_size = std::max(ver1[0].x, std::max(ver1[0].y, std::max(ver1[1].x, std::max(ver1[1].y, std::max(ver1[2].x, ver1[2].y)))));
	float obj2_max_size = std::max(ver2[0].x, std::max(ver2[0].y, std::max(ver2[1].x, std::max(ver2[1].y, std::max(ver2[2].x, ver2[2].y)))));
	obj1_max_size *= std::max(obj1.transform->scale.x, std::max(obj1.transform->scale.y, obj1.transform->scale.z))*3;
	obj2_max_size *= std::max(obj2.transform->scale.x, std::max(obj2.transform->scale.y, obj2.transform->scale.z))*3;
	if (pos_diff_dist > (obj1_max_size+obj2_max_size)*(obj1_max_size+obj2_max_size))
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

	const std::string name1 = obj1.model->GetName();
	const std::string name2 = obj2.model->GetName();

	const std::pair<std::string, std::string> colliders = { std::min(name1, name2), std::max(name1, name2) };

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

std::map<std::pair<std::string, std::string>, std::vector<std::function<void(Collisions::CollisionData*)>>> Collisions::callbacks;
void Collisions::addCallback(std::string a, std::string b, std::function<void(Collisions::CollisionData*)> callback) {
	callbacks[{std::min(a, b), std::max(a, b)}].push_back(callback);
}