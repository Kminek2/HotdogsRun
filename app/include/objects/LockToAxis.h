#pragma once
#include "ObjectScript.h"
#include "GameObject.h"

class LockToAxis :
	public ObjectScript
{
private:
	glm::vec3 axis;
	GameObject* obj;
public:
	LockToAxis(GameObject* obj, glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f));
	void Init() override;
	void Update() override;
	void OnDestroy() override;
};