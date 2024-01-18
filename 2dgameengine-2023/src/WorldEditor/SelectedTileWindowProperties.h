#pragma once
#include <string>
#include "../ECS/ECS.h"
struct SelectedTileWindowProperties {

public:
	bool isSelected;
	//int id;
	bool showSpriteChoices;
	Entity entity;
	//TransformComponent* tC;
	//SpriteComponent* sC;
	SelectedTileWindowProperties() : isSelected{ false }, showSpriteChoices{false}, entity() {};
	~SelectedTileWindowProperties()  {
		isSelected = NULL;
		showSpriteChoices = NULL;

	}


};