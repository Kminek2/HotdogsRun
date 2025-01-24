#pragma once
#include <memory>

#include "GameObject.h"
#include "Camera.h"
#include "Input.h"
#include "AppTime.h"

class Scene;

class SceeneScript
{
public:

	virtual std::shared_ptr<Scene> Init() = 0;

	virtual void Update() = 0;

	virtual void UnLoad() = 0;
};

