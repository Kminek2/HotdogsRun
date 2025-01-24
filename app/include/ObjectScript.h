#pragma once
#include "AppTime.h"
#include "Camera.h"
#include "Input.h"

class GameObject;

class ObjectScript
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void OnDestroy() = 0;

	GameObject* gameObject;
};