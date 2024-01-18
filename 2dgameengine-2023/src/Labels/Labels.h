#ifndef LABELS_H
#define LABELS_H
#include <unordered_map>
#include <string>

enum Tag {
	None,
	Player,
	Tile
};

enum Groups {
	GroupNone,
	UI,
	Enemies,
	Projectiles
};




struct TagMap {
public:
	// TODO jake make static!
	const std::unordered_map<Tag, std::string> tm{
		{Tag::None, "none"},
		{Tag::Player, "player"},
		{Tag::Tile, "tile"}
	};

	TagMap() {};

	std::unordered_map<Tag, std::string>::const_iterator Begin()  {
		return tm.begin();
	}

	std::unordered_map<Tag, std::string>::const_iterator End() {
		return tm.end();
	}

	const std::string at(Tag t) {
		return tm.at(t);
	}
};

struct GroupMap {
public: 
	// TODO jake make static!
	const std::unordered_map<Groups, std::string> gm{
		{Groups::GroupNone, "none"},
		{Groups::UI, "ui"},
		{Groups::Enemies, "enemies"},
		{Groups::Projectiles, "projectiles"}
	};

	GroupMap() {}

	std::unordered_map<Groups, std::string>::const_iterator Begin() {
		return gm.begin();
	}

	std::unordered_map<Groups, std::string>::const_iterator End() {
		return gm.end();
	}
	
	const std::string at(Groups g) {
		return gm.at(g);
	}
};

#endif