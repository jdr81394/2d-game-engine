#pragma once
#include "../ECS/ECS.h"
#include "../Labels/Labels.h"
#include <vector>

struct MouseSelectedTile {
public:

	MouseSelectedTile() = default;
	MouseSelectedTile(Entity entity, Tag tag = Tag::None, std::vector<Groups> groups = std::vector<Groups>()) :
		entity(entity), tag(tag), groups(groups)
	{};

	MouseSelectedTile(const MouseSelectedTile & t) = delete;
	MouseSelectedTile(MouseSelectedTile&& t) = delete;
	
	MouseSelectedTile& operator = (Entity e) = delete;

	Entity entity;
	Tag tag;
	std::vector<Groups> groups;


};