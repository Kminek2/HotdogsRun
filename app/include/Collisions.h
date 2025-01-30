#pragma once
#include "GameObject.h"
#include <cmath>

class Collisions
{
	private:
		static bool checkOBBCollision(const OBB& a, const OBB& b);
		static OBB getAbsOBB(const OBB& obb, const GameObject& obj);
		static glm::mat3 getRotationMatrix(glm::vec3 v);
	public:
		static bool checkCollision(const GameObject& obj1, const GameObject& obj2);
};