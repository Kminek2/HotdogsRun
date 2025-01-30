#include "scenes/MapDemo.h"
#include "Scene.h"
#include "mapgen.h"

std::shared_ptr<Scene> MapDemo::Init() {
	Scene* scene = new Scene(this);

	return std::shared_ptr<Scene>(scene);
}

void MapDemo::Update() {}

void MapDemo::UnLoad() {
	points.clear();
}
