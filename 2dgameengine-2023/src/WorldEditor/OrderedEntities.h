#pragma once
#include <vector>
#include <map>
//#include <map>
#include "../ECS/ECS.h"

class OrderedEntities {
private:
	//std::vector<Entity> orderedEntities; // Jake - refactored should be ordered by map to improve 
	std::map<int, std::map<int, std::vector<Entity>>> orderedEntities; // Jake - refactored should be ordered by map to improve 


public:
	OrderedEntities(): orderedEntities() {}
	~OrderedEntities() { 

		orderedEntities.clear();  // JAKE make refactor work with map
	}

	std::vector<Entity>& operator () (const int x, const int y) {
		return orderedEntities[x][y];
	}

	std::map<int, std::map<int, std::vector<Entity>>>::iterator inline Begin() {
		return orderedEntities.begin();
	}

	std::map<int, std::map<int, std::vector<Entity>>>::iterator inline End() {
		return orderedEntities.end();
	}

	void inline Add(const int x, const int y, const Entity& e) {
		orderedEntities[x][y].push_back(e);
	}
};