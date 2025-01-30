#include "Collisions.h"

bool Collisions::checkOBBCollision(const OBB& a, const OBB& b) {
	glm::mat3 rotation, absRotation;
	glm::vec3 distance = b.center - a.center;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			rotation[i][j] = glm::dot(a.axes[i], b.axes[j]);
		}
	}

	distance = glm::vec3(dot(distance, a.axes[0]), dot(distance, a.axes[1]), dot(distance, a.axes[2]));

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			absRotation[i][j] = std::abs(rotation[i][j]) + 1e-6f;
		}
	}

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

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			float ra = a.sizes[(i+1)%3]*absRotation[(i+2)%3][j] + a.sizes[(i+2)%3]*absRotation[(i+1)%3][j];
			float rb = b.sizes[(j+1)%3]*absRotation[i][(j+2)%3] + b.sizes[(j+2)%3]*absRotation[i][(j+1)%3];
			if (std::abs(distance[(i+1)%3]*rotation[(i+2)%3][j] - distance[(i+2)%3]*rotation[(i+1)%3][j] > ra + rb))
				return false;
		}
	}
	return true;
}

bool Collisions::checkCollision(const GameObject& obj1, const GameObject& obj2) {
	for (int i = 0; i < obj1.obbs.size(); ++i) {
		for (int j = 0; j < obj2.obbs.size(); ++j) {
			OBB absOBB1 = getAbsOBB(obj1.obbs[i], obj1);
			OBB absOBB2 = getAbsOBB(obj2.obbs[i], obj2);
			if (checkOBBCollision(absOBB1, absOBB2))
				return true;
		}
	}
	return false;
}

OBB Collisions::getAbsOBB(const OBB& obb, const GameObject& obj) {
	OBB absOBB;
	glm::mat3 rotationMatrix = getRotationMatrix(obj.transform->rotation);
	glm::vec3 absPosition = glm::vec3(obj.transform->position.x, obj.transform->position.y, obj.transform->position.z);

	absOBB.center = absPosition + rotationMatrix * obb.center;
	
	for (int i = 0; i < 3; ++i) {
		absOBB.axes[i] = rotationMatrix * obb.axes[i];
	}

	absOBB.sizes = obb.sizes;
	return absOBB;
}

glm::mat3 Collisions::getRotationMatrix(glm::vec3 rot) {
	glm::vec3 v = glm::radians(rot);
	float p = -v.x;
	float y = -v.y;
	float r = -v.z;

	glm::mat3 Rx(1.0f, 0.0f, 0.0f, 0.0f, cos(p), -sin(p), 0.0f, sin(p), cos(p));
	glm::mat3 Ry(cos(y), 0.0f, sin(y), 0.0f, 1.0f, 0.0f, -sin(y), 0.0f, cos(y));
	glm::mat3 Rz(cos(r), -sin(r), 0.0f, sin(r), cos(r), 0.0f, 0.0f, 0.0f, 1.0f);
	return Rx*Ry*Rz;
}