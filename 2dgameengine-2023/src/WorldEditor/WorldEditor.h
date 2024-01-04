#ifndef WORLDEDITOR_H
#define WORLDEDITOR_H
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/LeftMouseHeldDownEvent.h"
#include "../Events/LeftMouseClickedEvent.h"
#include "../Events/RightMouseClickedEvent.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"

#include "./OrderedTilesDataStructure.h"
#include "./TileMap.h"
#include "./Vec2.h"

#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <iostream>
#include <memory>
#include <fstream>

#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>


#include <string>





class WorldEditor {
private:
	Entity mouseSelectedTile;
	const std::unique_ptr<Registry>& registry;
	TileMap tileMap = {};
	OrderedTilesDataStructure* worldMap;
	std::map<std::string, std::string> assetIdToFilePath;
	bool generateWorld;
	Vec2 mouseGrabCoordinates;
	Vec2 worldDisplacement;

public:

	WorldEditor() = default;

	WorldEditor(const std::unique_ptr<Registry> & r) : registry(r) {
		this->mouseSelectedTile = NULL;
		this->tileMap.tileSize = 32;
		this->tileMap.scale = 2.0;
		this->generateWorld = false;
		this->worldMap = nullptr;
		this->mouseGrabCoordinates = {Vec2()};
		this->worldDisplacement = { Vec2() };

	}

	void inline setMouseGrabCoordinate(int x, int y) {
		mouseGrabCoordinates.x = x;
		mouseGrabCoordinates.y = y;
	}
 
	void inline setIdToFile(std::string assetId, std::string filePath) {
		assetIdToFilePath[assetId] = filePath;
	}

	void OnRightClick(RightMouseClickedEvent& event);

	void inline SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		//eventBus->SubscribeToEvent<LeftMouseClickedEvent>(this, &WorldEditorSystem::OnLeftClick);
		eventBus->SubscribeToEvent<LeftMouseHeldDownEvent>(this, &WorldEditor::OnLeftMouseHeldDown);
		eventBus->SubscribeToEvent<RightMouseClickedEvent>(this, &WorldEditor::OnRightClick);


	};
	void OnLeftMouseHeldDown(LeftMouseHeldDownEvent& event);
	//void OnLeftClick(LeftMouseClickedEvent& event);

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect& camera, SDL_Window* window);

private: 

	void GenerateFinalWorldMap(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Window* window);
	void RenderSelectWorldDimensionsWindow(TileMap& tileMap, bool& generateWorld);
	void GenerateGrid(SDL_Renderer* renderer, TileMap& tileMap, SDL_Rect& camera);
};

#endif


