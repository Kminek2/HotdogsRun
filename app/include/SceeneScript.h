#pragma once
#include <memory>

#include "UiObject.h"
#include "Camera.h"
#include "Input.h"
#include "AppTime.h"

// for convinience
#include "Application.h"
#include "LightObject.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "AudioSource2d.h"
#include "AudioSource3d.h"
#include "Text.h"

class Scene;

class SceeneScript
{
public:
	virtual std::shared_ptr<Scene> Init() = 0;
	virtual void EarlyUpdate() {};
	virtual void Update() = 0;
	virtual void LateUpdate() {};
	virtual void UnLoad() = 0;
};

