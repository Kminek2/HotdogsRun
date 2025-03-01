#pragma once

#include "SceeneScript.h"
#include "GameObject.h"
#include "Model.h"
#include "QuickCamera.h"
#include "_rand.hpp"
#include "mapgen.h"

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

	const float MAP_TILE_SIZE = 12.7f; // 127x127 mv size

	void add_decor(_rand rand, std::vector<mapgen::MapPoint> map_points);

public:
	std::shared_ptr<Scene> Init() override;
	void Update() override;
	void UnLoad() override;
};