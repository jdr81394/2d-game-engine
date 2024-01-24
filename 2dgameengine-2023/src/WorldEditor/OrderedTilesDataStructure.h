#ifndef ORDEREDTILESDATASTRUCTURE_H
#define ORDEREDTILESDATASTRUCTURE_H

#include "./TileMap.h"
#include "../ECS/ECS.h"
#include "./Vec2.h";
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "./OrderedEntities.h"

#include <map>



class OrderedTilesDataStructure {

private:
	// Have both these data structures for potential needs in the future.
	std::map<int, std::map<int, Entity>> tilesOrderedByX;
	std::map<int, std::map<int, Entity>> tilesOrderedByY;

public:

	OrderedTilesDataStructure() = default;

	OrderedTilesDataStructure(TileMap tileMap);


	inline std::map<int, std::map<int, Entity>> GetTilesOrderedByX() {
		return tilesOrderedByX;
	};

	inline std::map<int, std::map<int, Entity>> GetTilesOrderedByY() {
		return tilesOrderedByY;
	};

	void LogTiles();

	void RemoveTile(Vec2 coords);

	std::set<std::string> GetAllUniqueAssetIds();

	void AddTile(Entity e, Vec2 worldDisplacement);

	void UpdateTilePositionsByOffset(Vec2 worldDisplacement);

};

#endif