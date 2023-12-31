#ifndef ORDEREDTILESDATASTRUCTURE_H
#define ORDEREDTILESDATASTRUCTURE_H

#include "./TileMap.h"

#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"

#include <map>



class OrderedTilesDataStructure {

private:
	// Have both these data structures for potential needs in the future.
	std::map<int, std::map<int, Entity>> tilesOrderedByX;
	std::map<int, std::map<int, Entity>> tilesOrderedByY;

public:

	inline std::map<int, std::map<int, Entity>> GetTilesOrderedByX() {
		return tilesOrderedByX;
	};

	inline std::map<int, std::map<int, Entity>> GetTilesOrderedByY() {
		return tilesOrderedByY;
	};

	void LogTiles();

	OrderedTilesDataStructure() = default;

	OrderedTilesDataStructure(TileMap tileMap);

	std::set<std::string> GetAllUniqueAssetIds();

	void AddTile(Entity e);

};

#endif