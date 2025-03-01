#pragma once

#include "SceeneScript.h"
#include "GameObject.h"
#include "Model.h"
#include "QuickCamera.h"
#include "_rand.hpp"
#include "mapgen.h"
#include "objects/MapManager.h"

#include <utility>
#include <vector>

class MapDemo :
	public SceeneScript
{
private:
	QuickCamera* qc;
	MapManager* map;

	const float MAP_TILE_SIZE = 12.7f; // 127x127 mv size

public:
	std::shared_ptr<Scene> Init() override;
	void Update() override;
	void UnLoad() override;
};