#pragma once

#include "SceeneScript.h"
#include "GameObject.h"
#include "Model.h"
#include "QuickCamera.h"

#include <utility>
#include <vector>

class MapDemo :
	public SceeneScript
{
private:
	std::vector<GameObject*> points;
	std::vector<GameObject*> check_points;
	std::vector<GameObject*> mini_decors;
	std::vector<GameObject*> buildings;

	QuickCamera* qc;

public:
	std::shared_ptr<Scene> Init() override;
	void Update() override;
	void UnLoad() override;

	//void add_object();
};