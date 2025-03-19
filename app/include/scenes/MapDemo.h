#pragma once

#include "SceeneScript.h"
#include "GameObject.h"
#include "Model.h"
#include "_rand.hpp"
#include "mapgen.h"
#include "objects/MapManager.h"
#include "objects/BuildingManager.hpp"
#include "objects/RaceManager.hpp"

#include <utility>
#include <vector>

class MapDemo :
	public SceeneScript
{
private:
	MapManager* map;
	BuildingManager* build;
	RaceManager* race_manager;

	const float MAP_TILE_SIZE = 12.7f; // 127x127 mv size
	bool on_end_screen;
	void ShowEndScreen();
	bool UpdateEndScreen();

public:
	std::shared_ptr<Scene> Init() override;
	void Update() override;
	void UnLoad() override;

	void OnRaceEnd(RaceManager::CarObject* winner);
};