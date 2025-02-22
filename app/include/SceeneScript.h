#pragma once
#include <memory>

#include "UiObject.h"
#include "Camera.h"
#include "Input.h"
#include "AppTime.h"

// for convinience
#include "Application.h"

class Scene;

class SceeneScript
{
public:
	virtual std::shared_ptr<Scene> Init() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() {};
	virtual void UnLoad() = 0;
};

