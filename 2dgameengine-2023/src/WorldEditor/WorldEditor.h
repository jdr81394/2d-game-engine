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
#include "../Components/PlayerControlledComponent.h"
#include "../Components/KeyboardControlledComponent.h"


#include "./OrderedTilesDataStructure.h"
#include "./SelectedTileWindowProperties.h"
#include "./TileMap.h"
#include "./Vec2.h"
#include "./MouseSelectedTile.h"
#include "../Labels/Labels.h"
#include "../WorldEditor/OrderedEntities.h"

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
	MouseSelectedTile mouseSelectedTile;
	const std::unique_ptr<Registry>& registry;
	const std::unique_ptr<AssetStore>& assetStore;
	TileMap tileMap = {};
	std::map<std::string, std::string> assetIdToFilePath;
	bool generateWorld;
	Vec2 mouseGrabCoordinates;
	Vec2 worldDisplacement;
	SDL_Renderer * renderer;
	SelectedTileWindowProperties selectedTileWindowProperties;

	OrderedTilesDataStructure* worldMap;
	OrderedEntities orderedEntities;

	std::vector<std::string> tilesSelection; // Tiles in tab that one can choose 
	std::vector<std::string> entitiesSelection; // Entities in tab that we can choose

	void InitializeOrderedTilesDataStructure();
	void RenderTileSelectionWindow(SDL_Window* window);
	void RenderSelectedTileWindow();
	void GenerateFinalWorldMap(SDL_Window* window);
	void RenderSelectWorldDimensionsWindow(TileMap& tileMap, bool& generateWorld);
	void GenerateGrid(TileMap& tileMap, SDL_Rect& camera);
	void AddEntityToMap(Entity&entity);

public:

	WorldEditor() = default;

	WorldEditor(const std::unique_ptr<Registry> & r, SDL_Renderer* renderer, const std::unique_ptr<AssetStore>& assetStore)
		: registry(r), renderer(renderer), selectedTileWindowProperties{}, 
		tilesSelection(0), entitiesSelection(0), assetStore(assetStore),mouseSelectedTile{}, orderedEntities() {
		this->tileMap.tileSize = 32;
		this->tileMap.scale = 2.0;
		this->generateWorld = false;
		this->worldMap = nullptr;
		this->mouseGrabCoordinates = {Vec2()};
		this->worldDisplacement = { Vec2() };

	}

	void inline AddToTilesSelection(std::string&& assetId) {
		tilesSelection.push_back(assetId);
	}

	void inline AddToEntitiesSelection(std::string&& assetId) {
		entitiesSelection.push_back(assetId);
	}

	std::vector<std::string> inline GetEntitiesSelection() {
		return entitiesSelection;
	}

	std::vector<std::string> inline GetTilesSelection() {
		return tilesSelection;
	}

	void inline setMouseGrabCoordinate(int x, int y) {
		mouseGrabCoordinates.x = x;
		mouseGrabCoordinates.y = y;
	}
 
	void inline setIdToFile(std::string assetId, std::string filePath) {
		assetIdToFilePath[assetId] = filePath;
	}

	void RenderSelectedTileOutline();

	void OnRightClick(RightMouseClickedEvent& event);

	void inline SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		//eventBus->SubscribeToEvent<LeftMouseClickedEvent>(this, &WorldEditorSystem::OnLeftClick);
		eventBus->SubscribeToEvent<LeftMouseHeldDownEvent>(this, &WorldEditor::OnLeftMouseHeldDown);
		eventBus->SubscribeToEvent<RightMouseClickedEvent>(this, &WorldEditor::OnRightClick);


	};
	void OnLeftMouseHeldDown(LeftMouseHeldDownEvent& event);
	//void OnLeftClick(LeftMouseClickedEvent& event);

	void Render(SDL_Renderer* renderer, SDL_Rect& camera, SDL_Window* window);


};

#endif


