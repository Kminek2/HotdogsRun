#pragma once

#include "ObjectScript.h"
#include "GameObject.h"

class LockPosition :
	public ObjectScript
{
private:
	Transform* transform;
	glm::vec3 offset;

public:
	LockPosition(Transform* transform, glm::vec3 offset = glm::vec3(0.0f));
	void Init() override;
	void Update() override;
	void OnDestroy() override;
};