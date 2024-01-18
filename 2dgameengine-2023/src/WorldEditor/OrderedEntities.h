#pragma once
#include <vector>
//#include <map>
#include "../ECS/ECS.h"

class OrderedEntities {
private:
	std::vector<Entity> orderedEntities;

public:
	OrderedEntities(): orderedEntities() {}
	~OrderedEntities() { orderedEntities.clear(); }

	Entity operator [] (const int x) const {
		return orderedEntities[x];
	}

	std::vector<Entity>::iterator inline Begin() {
		return orderedEntities.begin();
	}

	std::vector<Entity>::iterator inline End() {
		return orderedEntities.end();
	}

	void inline Push(Entity e) {
		orderedEntities.push_back(e);
	}
};