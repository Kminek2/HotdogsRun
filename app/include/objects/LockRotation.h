#pragma once

#include "ObjectScript.h"
#include "GameObject.h"

class LockRotation :
	public ObjectScript
{
private:
	Transform* transform;
	glm::vec3 offset;

public:
	LockRotation(Transform* transform, glm::vec3 offset = glm::vec3(0.0f));
	void Init() override;
	void Update() override;
	void OnDestroy() override;

	LockRotation* copy() override { return new LockRotation(*this); }
};