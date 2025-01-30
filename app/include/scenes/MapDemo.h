#pragma once

#include "SceeneScript.h"
#include "GameObject.h"
#include "Model.h"

#include <utility>
#include <vector>

class MapDemo :
	public SceeneScript 
{
private:
	std::vector<GameObject*> points;

public:
	std::shared_ptr<Scene> Init() override;
	void Update() override;
	void UnLoad() override;
};