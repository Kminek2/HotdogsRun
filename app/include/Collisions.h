#pragma once
#include "GameObject.h"

#include <cmath>
#include <functional>

class ShowOBB;

class Collisions
{
public:
	struct CollisionData {
		const GameObject& obj1;
		const GameObject& obj2;
	};

	static bool checkCollision(const GameObject& obj1, const GameObject& obj2, bool callback = true);
	friend ShowOBB;

	static void addCallback(std::string a, std::string b, std::function<void(CollisionData*)> callback);

private:
	static bool checkOBBCollision(const OBB& a, const OBB& b);
	static OBB getAbsOBB(const OBB& obb, const GameObject& obj);
	static glm::mat3 getRotationMatrix(glm::vec3 v);

	static std::map<std::pair<std::string, std::string>, std::vector<std::function<void(CollisionData*)>>> callbacks;
};