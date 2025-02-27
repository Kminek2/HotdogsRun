#pragma once
#include "AppTime.h"
#include "Camera.h"
#include "Input.h"
#include "LightObject.h"

class GameObject;

class ObjectScript
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() {};
	virtual void OnDestroy() = 0;

	GameObject* gameObject;
};