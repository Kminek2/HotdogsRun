#pragma once
#include "AppTime.h"
#include "Camera.h"
#include "Input.h"
#include "LightObject.h"
#include "AudioSource2d.h"
#include "AudioSource3d.h"
#include "Text.h"

class GameObject;

class ObjectScript
{
public:
	virtual void Init() = 0;
	virtual void EarlyUpdate() {};
	virtual void Update() = 0;
	virtual void LateUpdate() {};
	virtual void OnDestroy() = 0;
	virtual ObjectScript* copy() = 0;

	GameObject* gameObject;
};