#include "./OrderedTilesDataStructure.h"


OrderedTilesDataStructure::OrderedTilesDataStructure(TileMap tileMap) {
	/*	Logger::Err("HEWorldMapDataStructureRE MF");
		Logger::Log("numRows: " + std::to_string(tileMap.numRows));
		Logger::Log("numCols: " + std::to_string(tileMap.numCols));*/

	// initialize first map of the tilesOrderedByY
	for (int i = 0; i < tileMap.numRows; i++) {
		// Logger::Log("COUNTER: " + std::to_string(i));
		tilesOrderedByY[i] = std::map<int, Entity>();
		std::map<int, Entity>& m = tilesOrderedByY[i];

		// Now for each of these maps, set it to a nullptr
		for (int j = 0; j < tileMap.numCols; j++) {

			m[j] = NULL;
		}
	}


	// initialize tilesOrderedByX;
	for (int i = 0; i < tileMap.numCols; i++) {
		tilesOrderedByX[i] = std::map<int, Entity>();
		std::map<int, Entity>& m = tilesOrderedByX[i];
		//Logger::Log("Tile map numrowS: " + std::to_string(tileMap.numRows));
		for (int j = 0; j < tileMap.numRows; j++) {
			//Logger::Log("J: " + std::to_string(j));
			m[j] = NULL;


		}

	}

	for (const auto& xEntry : tilesOrderedByX) {
		int xCoordinate = xEntry.first;
		const std::map<int, Entity>& yMap = xEntry.second;

		//Logger::Log("Xcoordinate :" + std::to_string(xCoordinate));

		for (const auto& yEntry : yMap) {
			int yCoordinate = yEntry.first;
			//Logger::Log("Ycoordinate :" + std::to_string(yCoordinate));

			Entity entity = yEntry.second;

			// Process entity or perform desired operations
			// Example: 
			if (entity != NULL) {
				//std::cout << "TilesOrderedByX" << "X: " << xCoordinate << ", Y: " << yCoordinate << ", Entity: " << entity.GetId() << std::endl;

			}
		}
	}

	// Loop through tilesOrderedByY
	//std::cout << "\nLooping through tilesOrderedByY:" << std::endl;
	for (const auto& yEntry : tilesOrderedByY) {
		int yCoordinate = yEntry.first;
		const std::map<int, Entity>& xMap = yEntry.second;

		for (const auto& xEntry : xMap) {
			int xCoordinate = xEntry.first;
			Entity entity = xEntry.second;

			// Process entity or perform desired operations
			// Example: 
			std::cout << "orderedBy Y" << "Y: " << yCoordinate << ", X: " << xCoordinate << ", Entity: " << entity.GetId() << std::endl;
		}
	}

}


void OrderedTilesDataStructure::LogTiles() {
	// Log out the contents of tilesOrderedByX
	for (const auto& xEntry : tilesOrderedByX) {
		int xCoordinate = xEntry.first;
		const std::map<int, Entity>& yMap = xEntry.second;

		for (const auto& yEntry : yMap) {
			int yCoordinate = yEntry.first;
			const Entity& entity = yEntry.second;

			// Process entity or perform desired operations
			// Example:
			//std::cout << "TilesOrderedByX" << "X: " << xCoordinate << ", Y: " << yCoordinate << ", Entity ID: " << entity.GetId() << std::endl;
		}
	}

	// Log out the contents of tilesOrderedByY
	for (const auto& yEntry : tilesOrderedByY) {
		int yCoordinate = yEntry.first;
		const std::map<int, Entity>& xMap = yEntry.second;

		for (const auto& xEntry : xMap) {
			int xCoordinate = xEntry.first;
			const Entity& entity = xEntry.second;

			// Process entity or perform desired operations
			// Example:
			//std::cout << "TilesOrderedByY" << "Y: " << yCoordinate << ", X: " << xCoordinate << ", Entity ID: " << entity.GetId() << std::endl;
		}
	}
}


std::set<std::string> OrderedTilesDataStructure::GetAllUniqueAssetIds() {
	std::set<std::string> result;

	for (const auto& xEntry : tilesOrderedByX) {
		int xCoordinate = xEntry.first;
		const std::map<int, Entity>& yMap = xEntry.second;

		for (const auto& yEntry : yMap) {
			int yCoordinate = yEntry.first;
			if (yEntry.second != NULL) {
				const Entity& entity = yEntry.second;

				std::cout << "ID: " << entity.GetId();
				// Process entity or perform desired operations
				// Example:
				//std::cout << "TilesOrderedByX" << "X: " << xCoordinate << ", Y: " << yCoordinate << ", Entity ID: " << entity.GetId() << std::endl;
				if (entity.HasComponent<SpriteComponent>()) {
					const SpriteComponent sC = entity.GetComponent<SpriteComponent>();
					std::string s = sC.assetId;
					if (!s.empty()) result.insert(s);

				}
			}


		}
	}

	// Jake - THIS SHOULDN'T BE NEEDED THEORETICALLY
	for (const auto& yEntry : tilesOrderedByY) {
		int yCoordinate = yEntry.first;
		const std::map<int, Entity>& xMap = yEntry.second;

		for (const auto& xEntry : xMap) {
			int xCoordinate = xEntry.first;
			if (xEntry.second != NULL) {

				const Entity& entity = xEntry.second;

				// Process entity or perform desired operations
				// Example:
				//std::cout << "TilesOrderedByY" << "Y: " << yCoordinate << ", X: " << xCoordinate << ", Entity ID: " << entity.GetId() << std::endl;
				if (entity.HasComponent<SpriteComponent>()) {
					const SpriteComponent sC = entity.GetComponent<SpriteComponent>();
					std::string s = sC.assetId;
					if (!s.empty()) result.insert(s);

					//result.insert(s);
					//result.insert(sC.assetId);

				}
			}

		}
	}

	return result;
}

// Assuming tilesOrderedByX and Y are in coordination
bool OrderedTilesDataStructure::HasTile(int x, int y) {
	
	try {
		const std::map<int, std::map<int, Entity>>& m = GetTilesOrderedByX();

		const std::map<int, Entity>& n = m.at(x);

		const Entity& e = n.at(y);

		if (e != NULL) {
			return true;
		}
		else {
			return false;
		}

	}
	catch (...) {
		Logger::Err("Tile does not exist at: " + std::to_string(x) + " " + std::to_string(y) + ". ");
		return false;
	}
}

void OrderedTilesDataStructure::RemoveTile(Vec2 coords) {

	try {
		std::map<int, Entity>& map1 = tilesOrderedByX.at(coords.x);
		Entity& e1 = map1.at(coords.y);

		std::map<int, Entity>& map2 = tilesOrderedByY.at(coords.y);
		Entity& e2 = map2.at(coords.x);

		if (e1 != NULL) {
			e1.Kill();
			map1.erase(coords.y);
			Logger::Log("KILLED 1");
		}

		if (e2 != NULL) {
			e2.Kill();
			map2.erase(coords.x);
			Logger::Log("KILLED 2");
		}
	}
	catch (const std::out_of_range& e) {
		Logger::Err("Key not found while removing tile, X:  " + std::to_string(coords.x) + " Y: " + std::to_string(coords.y));

	}



}


void OrderedTilesDataStructure::AddTile(Entity e, Vec2 worldDisplacement) {
	TransformComponent& tC = e.GetComponent<TransformComponent>();
	const SpriteComponent sC = e.GetComponent<SpriteComponent>();

	int eX = ceil((tC.position.x - worldDisplacement.x) / sC.width);			// This will get the exact index
	int eY = ceil((tC.position.y - worldDisplacement.y) / sC.height);

	// JAKE - upon initialization I should initialize each key that the map should have. That out of range error should never occur.

	// Check if entity exists at that place already
	// Go through the maps at the specific x and y value keys




	try {
		std::map<int, Entity>& map2 = tilesOrderedByX.at(eX);

		Entity e2 = map2.at(eY);

		if (e2 != NULL) {

			// Delete element
			map2.erase(eY);
			e2.Kill();
		}
	}
	catch (const std::out_of_range& e) {
		Logger::Err("Key not found in ordered by X: ");
	}

	try {
		std::map<int, Entity>& map2 = tilesOrderedByY.at(eY);
		Entity e2 = map2.at(eX);
		if (e2 != NULL) {
			map2.erase(eX);
			e2.Kill();
		}

	}
	catch (const std::out_of_range& e) {
		Logger::Err("Key not found in ordered by Y: ");
	}


	// Readjust 
	tC.position.x = eX * 32 + worldDisplacement.x;
	tC.position.y = eY * 32 + worldDisplacement.y;

	// Add the entity, go to the vector 
	tilesOrderedByX[eX][eY] = e;
	tilesOrderedByY[eY][eX] = e;

	//Logger::Log("id: " + std::to_string(tilesOrderedByX[eX][eY].GetId()));

}

void OrderedTilesDataStructure::UpdateTilePositionsByOffset(Vec2 worldDisplacement) {
	

	

	for (auto it = tilesOrderedByX.begin(); it != tilesOrderedByX.end(); ++it) {
		std::map<int, Entity>& map2 = it->second;
		for (auto it2 = map2.begin(); it2 != map2.end(); ++it2) {
			Entity& e = it2->second;

			if (e != NULL) {
				if (e.HasComponent<TransformComponent>()) {

					TransformComponent& tC = e.GetComponent<TransformComponent>();
					int x = it->first;
					int y = it2->first;
					// 32 represents tile size
					tC.position.x = (x * 32) + worldDisplacement.x;
					tC.position.y = (y * 32) + worldDisplacement.y;
				}
			}

		}
	}

	for (auto it = tilesOrderedByY.begin(); it != tilesOrderedByY.end(); ++it) {
		std::map<int, Entity>& map2 = it->second;
		for (auto it2 = map2.begin(); it2 != map2.end(); ++it2) {
			Entity& e = it2->second;

			if (e != NULL) {
				if (e.HasComponent<TransformComponent>()) {
					TransformComponent& tC = e.GetComponent<TransformComponent>();
					int y = it->first;
					int x = it2->first;
					// 32 represents tile size
					tC.position.x = (x * 32) + worldDisplacement.x;
					tC.position.y = (y * 32) + worldDisplacement.y;
					//tC.position.x += worldDisplacement.x;
					//tC.position.y += worldDisplacement.y;
				}
			}

		}
	}

}
