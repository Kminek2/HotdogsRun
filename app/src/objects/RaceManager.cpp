#include "objects/RaceManager.hpp"

#include <glm/vec3.hpp>

const std::array<glm::vec2, 4> RaceManager::offsets = std::array<glm::vec2, 4>({ glm::vec2(1, 1), glm::vec2(-1, 1), glm::vec2(-1, -1), glm::vec2(1, -1) });

RaceManager* RaceManager::SetMapManager(MapManager* map_manager) {
	this->map_manager = map_manager;
	return this;
}

RaceManager* RaceManager::SetCarsRelativeOffset(float offset) {
	this->cars_relative_offset = offset;
	return this;
}

/// <summary>
/// Return n normalized to [0, lim)
/// </summary>
int normalize(int n, int lim) {
	while (n < 0) n += lim;
	return n % lim;
}

/// <summary>
/// Place the car in the right starting position. Returns the id of the car
/// </summary>
RaceManager* RaceManager::AddCar(GameObject* car) {
	if (!map_manager) throw std::invalid_argument("init map_manager first");

	const int n = map_manager->GetLen();
	int point_id = normalize(-cars_placed / 4, n);

	glm::vec3 tile_points[2] = {
		map_manager->GetPoint(point_id)->transform->position,
		map_manager->GetPoint(normalize(point_id + 1, n))->transform->position
	};

	glm::vec2 tile_points_transformed[2] = {
		glm::vec2(tile_points[0].x, tile_points[0].y),
		glm::vec2(tile_points[1].x, tile_points[1].y)
	};

	float orient = atan2(
		tile_points_transformed[1].y - tile_points_transformed[0].y,
		tile_points_transformed[1].x - tile_points_transformed[0].x
	);

	car->transform->MoveTo(glm::vec3(tile_points_transformed[0].x, tile_points_transformed[0].y, 1));
	car->transform->RotateTo(glm::vec3(0, 0, glm::degrees(orient) - 180.0f));

	glm::vec2 offset = offsets[cars_placed % 4] * map_manager->GetMapScale() * map_manager->GetMapTileSize() * cars_relative_offset;
	car->transform->Translate(glm::vec3(offset.x, offset.y, 0));

 	++cars_placed;
	return this;
}