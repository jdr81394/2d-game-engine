#include "WorldEditor.h"


void WorldEditor::OnLeftMouseHeldDown(LeftMouseHeldDownEvent& event) {

	int x = event.GetX();
	int y = event.GetY();


	/* This is for moving the whole map */
	bool isShift = event.GetOtherKeys().isShift;

	if (isShift == true) {

		// Check difference in displacement
		int xDiff = x - mouseGrabCoordinates.x;
		int yDiff = y - mouseGrabCoordinates.y;

			//worldDisplacement.x = xDiff;a
			//worldDisplacement.y = yDiff;
			
		mouseGrabCoordinates.x = x;
		mouseGrabCoordinates.y = y;
		

		worldDisplacement.x += xDiff;
		worldDisplacement.y += yDiff;

		// If there is a difference, then you update the entities
		if (xDiff != 0 || yDiff != 0) {
			for (auto it = orderedEntities.Begin(); it != orderedEntities.End(); ++it) {
				const std::map<int, std::vector<Entity>> & map = it->second;

				for (auto jt = map.begin(); jt != map.end(); ++jt) {

					const std::vector<Entity>& v = jt->second;

					for (auto kt = v.begin(); kt != v.end(); ++kt) {
						const Entity& e = *kt;

						if (e.HasComponent<TransformComponent>()) {
							TransformComponent& tC = e.GetComponent<TransformComponent>();
							tC.position.x += xDiff;
							tC.position.y += yDiff;

							Logger::Log("X: " + std::to_string(tC.position.x) + "  Y: " + std::to_string(tC.position.y));
						}
					}

				}



			}
		}

		// Change the values of the tiles in the worldMap
		worldMap->UpdateTilePositionsByOffset(worldDisplacement);
	}
	else {
		mouseGrabCoordinates.x = x;
		mouseGrabCoordinates.y = y;
	}

	// Let's make sure that it is in bounds of the map, if not then just return
	if (x > (tileMap.numCols * tileMap.tileSize) + worldDisplacement.x || x <= 0 || x <= worldDisplacement.x) return;
	if (y > (tileMap.numRows * tileMap.tileSize) + worldDisplacement.y || y <= 0 || y <= worldDisplacement.y) return;

	/* This is for putting the tile down*/
	if (mouseSelectedTile.entity != NULL) {

		
		/*if (x > (tileMap.numCols * tileMap.tileSize) + worldDisplacement.x || x <= 0 || x <= worldDisplacement.x) return;
		if (y > (tileMap.numRows * tileMap.tileSize) + worldDisplacement.y || y <= 0 || y <= worldDisplacement.y ) return;*/

		/* We need to check to make sure an entity doesn't already exist so we don't place it down twice */

		//orderedEntities;


		/*
				auto isPointInsideSquare = [](const int tCX, const int tCY, const int tCW, const int tCH, const int x, const int y) -> bool {
					return (x >= tCX && x <= tCX + tCH &&
						y >= tCY && y <= tCY + tCH);
					};
		*/

		Entity entity = registry->CreateEntity();

		/* For position, let's make it snap into place */
		const SpriteComponent sC = mouseSelectedTile.entity.GetComponent<SpriteComponent>();
		const TransformComponent tC = mouseSelectedTile.entity.GetComponent<TransformComponent>();



		/*
			Check for tag before snapping
		*/

		glm::vec2 position{};

		if (mouseSelectedTile.tag == Tag::Tile) {
			int snapX = (floor(x / sC.width) * sC.width);
			int snapY = (floor(y / sC.height) * sC.height);

			if (snapX > tileMap.numCols * tileMap.tileSize - 32 + worldDisplacement.x) return;
			if (snapY > tileMap.numRows * tileMap.tileSize - 32 + worldDisplacement.y) return;

			position = glm::vec2(snapX, snapY);

		}
		else {
			// if not a tile, no need to snap, lets put it right in the middle of the mouse though
			position.x = x - (tileMap.tileSize / 2);
			position.y = y - (tileMap.tileSize / 2);
		}



		TagMap tagMap{};	// instatiate a tag map
		GroupMap groupMap{};

		glm::vec2&& scale(glm::vec2(tileMap.scale, tileMap.scale));

		entity.AddComponent<TransformComponent>(position);
		entity.AddComponent<SpriteComponent>(sC.assetId, 32, 32, 2);
		try {
			entity.Tag(tagMap.at(mouseSelectedTile.tag));
		}
		catch (std::out_of_range& e) {
			Logger::Log("Error, tag is not found: " + std::to_string(mouseSelectedTile.tag));
		}

		try {
			for (auto it = mouseSelectedTile.groups.begin(); it != mouseSelectedTile.groups.end(); it++) {
				entity.Group(groupMap.at(*it));
			}
		}
		catch (std::out_of_range& e) {
			Logger::Log("Error, group is not found... ");
		}


		if (mouseSelectedTile.tag == Tag::Tile) {
			worldMap->AddTile(entity, worldDisplacement);
		}
		else {
			// TODO jake - handle differnet kinds of entities but for now just create entity
			AddEntityToMap(position.x, position.y, entity);
		}

	}



	/* 
		1.This is for selecting a tile that is already laid down OR ENTITY
	*/	
	if (generateWorld == true && mouseSelectedTile.entity == NULL && isShift == false &&(selectedTileWindowProperties.isSelected == false || selectedTileWindowProperties.isSelected == NULL)) {

		// Let's run through the vector of entities and see if i can find it

		for (auto it = orderedEntities.Begin(); it != orderedEntities.End(); ++it) {
			/* lets see if its in hitbox using aabb */
			const std::map<int,std::vector<Entity>>& m = it->second;
			
			
			for (auto jt = m.begin(); jt != m.end(); ++jt) {

				const std::vector<Entity>& v = jt->second;

				for(auto kt = v.begin(); kt != v.end(); ++kt) {

					const Entity& e = *kt;

				
					if (e.HasComponent<TransformComponent>()) {

						const TransformComponent& tC = e.GetComponent<TransformComponent>();

						const int tCX = tC.position.x;
						const int tCY = tC.position.y;
						const int tCW = 32;		// use 32 for rn
						const int tCH = 32;

						// Jake return
						auto isPointInsideSquare = [](const int tCX, const int tCY, const int tCW, const int tCH, const int x, const int y) -> bool {
							return (x >= tCX && x <= tCX + tCH &&
								y >= tCY && y <= tCY + tCH);
							};

						// If the point is within the square, then assign the mouseSelectedTile to the entry
						if (isPointInsideSquare(tCX, tCY, tCW, tCH, x, y)) {
							selectedTileWindowProperties.entity = e;
							selectedTileWindowProperties.isSelected = true;
						}

					}

				}

			

			}
			
		
		}

		// If it is still false, that means that it was not an entity that was found so we should look for a tile
		if (selectedTileWindowProperties.isSelected == false) {

			// I need to determine what tile I am clicking on by where the mouse is in relation to the x and y axis
			const int eX(ceil((x - worldDisplacement.x) / tileMap.tileSize));
			const int eY(ceil((y - worldDisplacement.y) / tileMap.tileSize));

			/**
			First thing to do: Highlight selected sprite
			Idea 1:
				Create a highlight sprite, create entity as highlight sprite then delete it when its appropriate

			*/


			/**
				Second thing to do:
				Create a sprite window that will hold this
			*/

			// Get entityId
			// **** LATER *****
			// TODO Check if im clicking on an entity on top of the tile first - this will be for obstacles and enemies
			// Maybe create a map, like i did with X/yordered Tiles. Search the specific X value. If not there, search in a range of -16/+16
			// 
			// 
			// Go through each range, and for object see if the click is within it's x,y,height,width boundaries

			// Get entity out of the worldMap data structure first, then use it's id to get it out of the registry because we need to alter that, then we'll clean up the worldMap data structure later
			const Entity entity = worldMap->GetTilesOrderedByX()[eX][eY];

			if (entity != NULL) {
				selectedTileWindowProperties.entity = entity;

				selectedTileWindowProperties.isSelected = true;

			}

		}



	}


}

// This adds the entity to the .lua file and pushes a refernce to the entity to be held by the world editor
void WorldEditor::AddEntityToMap(int x, int y, Entity& entity) {
	Logger::Log("Size of vector before being added: " + std::to_string(orderedEntities[x][y].size()) + "\n");

	orderedEntities.Add(x,y,entity);
}

void WorldEditor::InitializeOrderedTilesDataStructure() {
	if (worldMap == nullptr) {
		worldMap = new OrderedTilesDataStructure(tileMap);
	}
}

void WorldEditor::RenderTileSelectionWindow(SDL_Window* window) {
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::Begin("Tile Selection", NULL, windowFlags);

	if (ImGui::Button("Log out World Map")) {
		worldMap->LogTiles();

	}

	ImGui::NewLine(); // Put space between Button and the rest of the tiles

	if (ImGui::Button("Create Map")) {
		GenerateFinalWorldMap(window);
	}

	ImGui::NewLine();

	ImGuiTabBarFlags tabFlags = ImGuiTabBarFlags_FittingPolicyResizeDown | ImGuiTabBarFlags_FittingPolicyScroll;
	ImGuiTabItemFlags itemTabFlags = ImGuiTabItemFlags_SetSelected;
	/*	bool* open = new bool(true);
		bool* closed = new bool(false);*/

		// Load Textures into Selection Pane
	const std::map<std::string, SDL_Texture*>& allTextures = assetStore->GetAllTextures(); //  type std::map<std::string, SDL_Texture*>

	// Shared variables
	ImVec2 size = ImVec2(tileMap.tileSize, tileMap.tileSize); // ImVec2& - size - width and height
	int currentCount{ 0 };
	const ImVec2 spacing{ 0.0f, 5.0f };

	ImGui::BeginTabBar("Tabs", tabFlags);
	if (ImGui::BeginTabItem("Tiles")) {

		ImGui::NewLine();

		const std::vector<std::string>& allTiles = GetTilesSelection();

		for (auto it = allTiles.begin(); it != allTiles.end(); ++it) {


			const std::string& tileAssetId = *it;

			SDL_Texture* texture = allTextures.at(tileAssetId);

			ImTextureID* textureId = reinterpret_cast<ImTextureID*>(texture);

			/*ImTextureID textureId = it->second;*/


			if (currentCount != 4) {
				ImGui::SameLine(spacing.x, spacing.y);
				currentCount++;
			}
			else {
				currentCount = 0;
			}

			// Get ready to move the tile 
			ImGuiIO& io = ImGui::GetIO();

			int mX, mY;

			// SDL function that gets the mouse state and puts it in these variables
			SDL_GetMouseState(&mX, &mY);

			if (ImGui::ImageButton(texture, size)) {
				if (mouseSelectedTile.entity != NULL) {
					registry->KillEntity(mouseSelectedTile.entity);
					mouseSelectedTile.entity = NULL;
				}
				mouseSelectedTile.entity = registry->CreateEntity();
				mouseSelectedTile.tag = Tag::Tile;
				TagMap tm{};

				mouseSelectedTile.entity.Tag(tm.at(mouseSelectedTile.tag));
			}

			if (mouseSelectedTile.entity != NULL) {

				/* Putting this here kind of fixes the bug with the first click. Doesn't create anything but prevents it from showing up */
				if (!mouseSelectedTile.entity.HasComponent<TransformComponent>()) {
					glm::vec2 position = glm::vec2(mX, mY);
					mouseSelectedTile.entity.AddComponent<TransformComponent>(position);
				}

				if (!mouseSelectedTile.entity.HasComponent<SpriteComponent>()) {
					mouseSelectedTile.entity.AddComponent<SpriteComponent>(tileAssetId, 32, 32, 100);
					Logger::Log("asset id: " + tileAssetId);
				}
				/* ---------------------------------------------------------------------------------------------------  */


				// Update it's position based on the users mouse
				TransformComponent& tc = mouseSelectedTile.entity.GetComponent<TransformComponent>();
				tc.position.x = mX - (tileMap.tileSize / 2);	// tileMap.tileSize - 2 puts it right in the center of the users mouse
				tc.position.y = mY - (tileMap.tileSize / 2); 

			}



		}


		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Entities")) {
		const std::vector<std::string>& allEntitesTiles = GetEntitiesSelection();

		ImGui::NewLine();

		for (auto it = allEntitesTiles.begin(); it != allEntitesTiles.end(); ++it) {
			const std::string& tileAssetId = *it;

			SDL_Texture* texture = allTextures.at(tileAssetId);

			ImTextureID* textureId = reinterpret_cast<ImTextureID*>(texture);

			if (currentCount != 4) {
				ImGui::SameLine(spacing.x, spacing.y);
				currentCount++;
			}
			else {
				currentCount = 0;
			}

			// Get ready to move the tile 
			ImGuiIO& io = ImGui::GetIO();

			int mX, mY;
			SDL_GetMouseState(&mX, &mY);

			if (ImGui::ImageButton(texture, size)) {
				if (mouseSelectedTile.entity != NULL) {
					registry->KillEntity(mouseSelectedTile.entity);
					mouseSelectedTile.entity = NULL;
				}
				mouseSelectedTile.entity = registry->CreateEntity();
				mouseSelectedTile.tag = Tag::None;
				TagMap tm{};

				mouseSelectedTile.entity.Tag(tm.at(mouseSelectedTile.tag));

			}
			if (mouseSelectedTile.entity != NULL) {

				/* Putting this here kind of fixes the bug with the first click. Doesn't create anything but prevents it from showing up */
				if (!mouseSelectedTile.entity.HasComponent<TransformComponent>()) {
					glm::vec2 position = glm::vec2(mX, mY);
					mouseSelectedTile.entity.AddComponent<TransformComponent>(position);
				}

				if (!mouseSelectedTile.entity.HasComponent<SpriteComponent>()) {
					mouseSelectedTile.entity.AddComponent<SpriteComponent>(tileAssetId, 32, 32, 100);
					Logger::Log("asset id: " + tileAssetId);
				}
				/* ---------------------------------------------------------------------------------------------------  */


				// Update it's position based on the users mouse
				TransformComponent& tc = mouseSelectedTile.entity.GetComponent<TransformComponent>();
				tc.position.x = mX - (tileMap.tileSize / 2);	// tileMap.tileSize - 2 puts it right in the center of the users mouse
				tc.position.y = mY - (tileMap.tileSize / 2);

			}





		}

		ImGui::EndTabItem();

	};
	ImGui::EndTabBar();


	// Generate the world 
	ImGui::End();

	//delete open;
	//delete closed;

}

void WorldEditor::RenderSelectedTileWindow() {
	/* Create a selected Tile window if needed */
	if (selectedTileWindowProperties.isSelected == true) {
		if (ImGui::Begin("Selected Tile", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Components");
			ImGui::NewLine();

			/* START OF SPRITE COMPONENTS */
			Entity entity = selectedTileWindowProperties.entity;

			if (entity.HasComponent<SpriteComponent>()) {

				SpriteComponent& sC = entity.GetComponent<SpriteComponent>();

				ImTextureID* t1 = reinterpret_cast<ImTextureID*>(assetStore->GetTexture(sC.assetId));
				ImVec2 size = ImVec2(tileMap.tileSize, tileMap.tileSize); // ImVec2& - size - width and height

				ImGui::SameLine(50, 0);

				ImGui::Image(t1, size);

				ImGui::Text("Sprite Component");

				if (ImGui::Button("Show Tile Options", size)) {
					selectedTileWindowProperties.showSpriteChoices = !selectedTileWindowProperties.showSpriteChoices;
				}

				if (selectedTileWindowProperties.showSpriteChoices) {
					const ImVec2&& spacing{ 0.0f, 5.0f };


					// tilesSelection
					/* Now create all selectable tile styles  */
					// if tileSelection is undefined
					for (int i{ 0 }; i < tilesSelection.size(); i++) {

						if (i % 8 != 0) {
							ImGui::SameLine(spacing.x, spacing.y);
						}

						ImTextureID* t2 = reinterpret_cast<ImTextureID*>(assetStore->GetTexture(tilesSelection[i]));
						ImVec2 size = ImVec2(tileMap.tileSize / 3, tileMap.tileSize / 3); // ImVec2& - size - width and height

						// If selected then change the sprites 
						if (ImGui::ImageButton(t2, size)) {
							sC.assetId = tilesSelection[i];
							Logger::Log("HERE!!!!");
						}

					}
				}
				ImGui::NewLine();
			}



			/* Start of Transform Component


			if (tC != nullptr) {
				ImGui::Text("Transform Component");
				const ImVec2&& spacing{ 10.0f, 0.0f };

				ImGui::SameLine(spacing.x, spacing.y);
				float w = tileMap.numCols * tileMap.tileSize - 32;	// - 32 to get it onto the right spot?

				if (ImGui::SliderFloat("X Position", &tC->position.x, 0, w)) {
					tC->position.x = (floor(tC->position.x / tileMap.tileSize) * tileMap.tileSize); // snap X into grid

					// TOOD: must adjust the x position in the registry component and the tilesOrderedByX/Y
					const int eX(ceil((tC->position.x - worldDisplacement.x) / tileMap.tileSize));
					const int eY(ceil((tC->position.y - worldDisplacement.y) / tileMap.tileSize));

					worldMap->RemoveTile(Vec2(eX, eY));

					Entity e = registry->Get							//problem, need entity because then ill have to recreate the tile every single time with each one of its components

					worldMap->AddTile(e, worldDisplacement);
				}
			}
			*/

			/* RigidBody Component - not sure if working - MUST EDIT LUA SCRIPT TO MAKE THIS WORK */

			ImGui::Text("Rigid Body");
			ImGui::NewLine();
			bool hasRigidBody = entity.HasComponent<RigidBodyComponent>();

			if (hasRigidBody == true) {
				RigidBodyComponent& rBC = entity.GetComponent<RigidBodyComponent>();

				ImGui::InputFloat("X Velocity:", &rBC.velocity.x, 1, 100);
				ImGui::InputFloat("Y Velocity:", &rBC.velocity.y, 1, 100);

				const TransformComponent& tC = entity.GetComponent<TransformComponent>();

				if (ImGui::Button("Remove RigidBody", ImVec2(100, 50))) {
					registry->RemoveComponent<RigidBodyComponent>(entity);
				};
			}
			else {
				if (ImGui::Checkbox("Add a RigidBody", &hasRigidBody)) {
					registry->AddComponent<RigidBodyComponent>(entity);
				};
			}


			ImGui::Text("Box Collider");
			ImGui::NewLine();
			bool hasCollider = entity.HasComponent<BoxColliderComponent>();
			// Check if it has collider
			if (hasCollider == true) {
				BoxColliderComponent& bC = entity.GetComponent<BoxColliderComponent>();

				ImGui::InputInt("Width:", &bC.width, 1, 100);
				ImGui::InputInt("Height:", &bC.height, 1, 100);

				ImGui::InputFloat("x offset:", &bC.offset.x, 1, 100);
				ImGui::InputFloat("y offset:", &bC.offset.y, 1, 100);

			
				if (ImGui::Button("Remove Collider", ImVec2(100, 50))) {
					registry->RemoveComponent<BoxColliderComponent>(entity);
				};
			}
			else {
				if (ImGui::Checkbox("Add a Colliider", &hasCollider)) {
					registry->AddComponent<BoxColliderComponent>(entity);
				};

			}

			// Add a Player controlled componet
			bool isPlayerControlled = entity.HasComponent<PlayerControlledComponent>();
			ImGui::Text("Player Controlled Component");
			ImGui::NewLine();
			if (isPlayerControlled) {
				if (ImGui::Checkbox("Remove Player Control", &isPlayerControlled)) {
					registry->AddComponent<PlayerControlledComponent>(entity);
				};
				ImGui::Text("Choose your type of control:");
				ImGui::NewLine();

				/* Render Keyboard controller */
				bool isKeyboardControlled = entity.HasComponent<KeyboardControlledComponent>();
				if (isKeyboardControlled) {
					KeyboardControlledComponent& kCC = entity.GetComponent<KeyboardControlledComponent>();

					ImGui::Text("Set velocity from keys");
					ImGui::NewLine();
					//ImGui::InputFloat("Up X Velocity:", &kCC.upVelocity.x, -1, 100);
					ImGui::InputFloat("Up Y Velocity:", &kCC.upVelocity.y, -1, -100);
					ImGui::NewLine();
					//ImGui::InputFloat("Down X Velocity:", &kCC.downVelocity.x, 1, 100);
					ImGui::InputFloat("Down Y Velocity:", &kCC.downVelocity.y, 1, 100);
					ImGui::NewLine();
					ImGui::InputFloat("Left X Velocity:", &kCC.leftVelocity.x, -1, -100);
					//ImGui::InputFloat("Left Y Velocity:", &kCC.leftVelocity.y, 1, 100);
					ImGui::NewLine();
					ImGui::InputFloat("Right X Velocity:", &kCC.rightVelocity.x, 1, 100);
					//ImGui::InputFloat("Right Y Velocity:", &kCC.rightVelocity.y, 1, 100);

					ImGui::NewLine();
					if (ImGui::Button("Remove Keyboard Control", ImVec2(100, 50))) {
						registry->RemoveComponent<KeyboardControlledComponent>(entity);
					};

				}
				else {
					if (ImGui::Button("Make Keyboard Control", ImVec2(100, 50))) {
						registry->AddComponent<KeyboardControlledComponent>(entity);
					};
				}


	
			}
			else {
				if (ImGui::Checkbox("Make it Player Controlled", &isPlayerControlled)) {
					registry->AddComponent<PlayerControlledComponent>(entity);
				};
			}

			ImGui::NewLine();


			ImGui::Text("Health Component");
			ImGui::NewLine();
			bool hasHealthComponent = entity.HasComponent<HealthComponent>();
			if (hasHealthComponent) {
				HealthComponent& hC = entity.GetComponent<HealthComponent>();
				ImGui::InputInt("Total HP: ", &hC.healthPercentage);

				if (ImGui::Button("Remove Health", ImVec2(100, 50))) {
					registry->RemoveComponent<HealthComponent>(entity);
				}
			}
			else {
				if(ImGui::Checkbox("Add Health", &hasHealthComponent)) {
					registry->AddComponent<HealthComponent>(entity);
				}
			}


			/*
			TODO:
			1. Animation (Tricky)
			2. Projectile
			3. Projectile Emitter
			4. Script
			5. TextLabel
			*/

			ImGui::End();
		}


	}

}


void WorldEditor::RenderSelectedTileOutline() {
	if(selectedTileWindowProperties.isSelected == true) {
		/* Render the entity selected square, render its collision, if not then just where it is on the map */
		bool hasTransform = selectedTileWindowProperties.entity.HasComponent<TransformComponent>();
		bool hasBoxCollider = selectedTileWindowProperties.entity.HasComponent<BoxColliderComponent>();
		if (hasBoxCollider) {
			const TransformComponent& tC = selectedTileWindowProperties.entity.GetComponent<TransformComponent>();
			const BoxColliderComponent& bCC = selectedTileWindowProperties.entity.GetComponent<BoxColliderComponent>();

			SDL_Rect highlight = { tC.position.x + bCC.offset.x, tC.position.y + bCC.offset.y, bCC.width, bCC.height };

			SDL_SetRenderDrawColor(renderer, 255,0, 0, 255);
			SDL_RenderDrawRect(renderer, &highlight); // This function draws an unfilled rectangle


		}
		else if (hasTransform && hasBoxCollider) {
			const TransformComponent& tC = selectedTileWindowProperties.entity.GetComponent<TransformComponent>();

			SDL_Rect highlight = { tC.position.x, tC.position.y, 32, 32 };

			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_RenderDrawRect(renderer, &highlight); // This function draws an unfilled rectangle
		}
	}


}

void WorldEditor::Render(SDL_Renderer* renderer, SDL_Rect& camera, SDL_Window* window) {
	ImGui::NewFrame();


	// Display a window for the tilemap:

	RenderSelectWorldDimensionsWindow(tileMap, generateWorld);


	if (generateWorld) {
		InitializeOrderedTilesDataStructure();
		GenerateGrid(tileMap, camera); // Generate Grid for dropping tiles onto
		RenderTileSelectionWindow(window);
	}

	RenderSelectedTileWindow();



	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
}


void WorldEditor::GenerateFinalWorldMap(SDL_Window* window) {

	// Keep track all the images that have been used
	std::set<std::string> allAssetIds;
	std::vector<SDL_Texture*> allTextures;

	// 1. get all the assetIds
	allAssetIds = worldMap->GetAllUniqueAssetIds();


	// 2. then use the asset ids to fetch all the textures, when i am doing this
	for (auto it = allAssetIds.begin(); it != allAssetIds.end(); ++it) {
		SDL_Texture* newTexture = assetStore->CopyTexture(renderer, assetStore->GetTexture(*it), window);
		allTextures.push_back(newTexture);
	}

	// 3. stitch all the textures into a jpg, based on the height and width of the tileMap. Must keep track of its x and y because that will be used in the map. Can use 10 cols by 10 (infi)
	// Determine numCols and numRows first. 
	// numCols = 10 unless its less than 10 textures
	// numRows = floor(numTextures / 10)
	int numCols = 10;
	int numRows = floor(allTextures.size() / 10);

	// In case it's 0, just set it to 1, in other case must increment it by 1 to make it the right number of rows
	numRows += 1;

	int widthOfSurface = numCols * 32;
	int heightOfSurface = numRows * 32;

	SDL_Surface* resultSurface = SDL_CreateRGBSurface(0, widthOfSurface, heightOfSurface, 32, 0, 0, 0, 0);


	if (!resultSurface) {
		std::cerr << "Failed to create result surface: " << SDL_GetError() << std::endl;
		return;
	}


	SDL_Rect dstRect = { 0, 0, 32, 32 };

	Uint32* pixels = new Uint32[32 * 32 * 32];

	for (auto i = 0; i < allTextures.size(); i++) {
		SDL_SetRenderTarget(renderer, allTextures[i]);
		if (SDL_RenderReadPixels(renderer, nullptr, 0, pixels, 32 * 4) != 0) {
			std::cerr << "Failed to read pixels: " << SDL_GetError() << std::endl;
		};
		if (SDL_Surface* temp = SDL_CreateRGBSurfaceFrom(pixels, 32, 32, 32, 32 * 4, 0xFF0000, 0x00FF00, 0x0000FF, 0xFF000000)) {
			if (SDL_BlitSurface(temp, nullptr, resultSurface, &dstRect) != 0) {
				std::cerr << "Failed to blit surface: " << SDL_GetError() << std::endl;
			}
			dstRect.x += 32;
			if (dstRect.x >= widthOfSurface) {
				dstRect.x = 0;
				dstRect.y += 32;
			}
		}
		else {
			std::cerr << "Failed to create  RGB From pixels: " << SDL_GetError() << std::endl;
		}


	}

	// Save the result surface as a .jpg file using SDL_image
	std::string s = R"(C:\Users\17329\source\repos\2dgameengine-2023\2dgameengine-2023\assets\tilemaps\test.png)";
	if (IMG_SavePNG(resultSurface, s.c_str()) != 0) {
		std::cerr << "Failed to save image: " << IMG_GetError() << std::endl;
	}
	SDL_FreeSurface(resultSurface);




	/*

		4. Put that into the map
		4a. Loop through all the tiles ordered by x or by y.
		4b. Get their assetId. Then, loop through all the assetIds until I find the one that matches.
		4c. When it matches, determine the y and x value by the current count
		4d. Write that y and x value to the file
	*/

	std::string s2 = R"(C:\Users\17329\source\repos\2dgameengine-2023\2dgameengine-2023\assets\tilemaps\test.map)";
	std::ofstream outputFile;
	outputFile.open(s2, std::ios::trunc);
	int textMapCount{ 0 };

	if (outputFile.is_open()) {
		const std::map<int, std::map<int, Entity>>  map = worldMap->GetTilesOrderedByY();
		for (auto it = map.begin(); it != map.end(); ++it) {

			const std::map<int, Entity> map2 = it->second;
			for (auto it2 = map2.begin(); it2 != map2.end(); ++it2) {
				// Make sure entity exist
				if (it2->second != NULL) {
					const Entity& e = it2->second;


					if (e.HasComponent<SpriteComponent>()) {
						const SpriteComponent sC = e.GetComponent<SpriteComponent>();

						if (!sC.assetId.empty()) {
							const std::string assetId = sC.assetId;

							/* 4b step: */
							int count = 0;
							bool isFound = false;
							for (auto aIt = allAssetIds.begin(); aIt != allAssetIds.end(); aIt++) {

								// If id was found, then we know where it was in the .png file and we can break the loop.
								if (assetId == *aIt) {
									isFound = true;
									break;
								}
								count++;
							}

							// Flag to catch error
							if (isFound == false) {
								Logger::Err("asset Id was never found");
								return;
							}

							/* 4c step: */
							// Declare variables
							int x;
							int y;

							// If count is less than 10 or equal to 10, we know y is just 0, and x is whatever the count is
							if (count <= 9) {
								x = count;
								y = 0;
							}
							else if (count >= 10 && count < 20){
								// how many times does 10 go into the count?
								
								x = count - 10;
								y = 1;
							}
							else if (count >= 20 && count < 30) {
								// how many times does 10 go into the count?

								x = count - 20;
								y = 2;
							}
							else if (count >= 30 && count < 40) {
								// how many times does 10 go into the count?

								x = count - 30;
								y = 3;
							}
							else if (count >= 40 && count < 50) {
								// how many times does 10 go into the count?

								x = count - 40;
								y = 4;
							}
							else if (count >= 50 && count < 60) {
								// how many times does 10 go into the count?

								x = count - 50;
								y = 5;
							}
							else if (count >= 60 && count < 70) {
								// how many times does 10 go into the count?

								x = count - 60;
								y = 6;
							}

							
							else {
								Logger::Err("Count for number of tiles was too great! Here is the count: " + std::to_string(count));
							}

							// Will break after this was just a fast way to code this properly.

							/* 4d step: */

							if (std::next(it2) != map2.end()) {
								outputFile << y << x << ",";
							}
							else {
								outputFile << y << x;
							}

							if (textMapCount == (tileMap.numCols - 1)) {
								outputFile << "\n";
								textMapCount = 0;
							}
							else {
								textMapCount++;
							}


						}
						else {
							Logger::Err("asset id was empty");
						}
					}
					else {
						Logger::Err("entity did not have component");
					}
				}
				else {
					Logger::Log("Add XX");

					if (std::next(it2) != map2.end()) {
						outputFile << "XX,";
					}
					else {
						outputFile << "XX";
					}

					if (textMapCount == (tileMap.numCols - 1)) {
						outputFile << "\n";
						textMapCount = 0;
					}
					else {
						textMapCount++;
					}

				}

			}
		}

	}


	outputFile.close();


	/*
	cleanup
	1. destroy ALL the textures after
	2. destroy pixel pointer

	*/

	delete[] pixels;

	for (const auto& texture : allTextures) {
		SDL_DestroyTexture(texture);
	}

	SDL_SetRenderTarget(renderer, NULL);




	/*
		5. Generate the script file
		5a. For each assetId link it to the file - DONE with the assetIdToFilePath
		5b. Get each filePath that is important
		5c. Put it into the lua file

	*/

	std::string final = "";

	final += "Level = {\n";	// open level bracket
	final += "\tassets = {\n";// open asset
	final += "\t\t[0] = \n";		// asset first index
	final += "\t\t{ type = \"texture\" , id = \"./assets/tilemaps/test\",  file = \"./assets/tilemaps/test.png\" },\n";
	
	for (auto it = orderedEntities.Begin(); it != orderedEntities.End(); ++it) { // add all assets for units
		
		const std::map<int, std::vector<Entity>>&  map = it->second;

		for (auto jt = map.begin(); jt != map.end(); ++jt) {
		
			const std::vector<Entity>& v = jt->second;

			for (auto kt = v.begin(); kt != v.end(); ++kt) {
				
				const Entity& e = *kt;

				if (e.HasComponent<SpriteComponent>()) {
					const SpriteComponent& sC = e.GetComponent<SpriteComponent>();
					const std::string&& path = std::move(assetIdToFilePath[sC.assetId]);

					final += "\t\t{ type = \"texture\", id=\"" + sC.assetId + "\",	file =\"" + path + "\" },\n";
				}
			}


		}
		

	}

	
	final += "\t}\n,";	// close assets
	final += "\ttilemap = {\n";	// open tile map
	final += "\t\tmap_file=\"./assets/tilemaps/test.map\",\n";
	final += "\t\ttexture_asset_id=\"./assets/tilemaps/test\",\n";
	final += "\t\tnum_rows=" + std::to_string(tileMap.numRows) + ",\n";
	final += "\t\tnum_cols=" + std::to_string(tileMap.numCols) + ",\n";
	final += "\t\ttile_size=32,\n";
	final += "\t\tscale=2.0\n";
	final += "\t},\n"; // close tile map

	final += "\tentities = {\n";	// open entities
	final += "\t\t[0] = ";

	/* Handle all entities and their components*/

	for (auto it = orderedEntities.Begin(); it != orderedEntities.End(); ++it) {

		const std::map<int, std::vector<Entity>>& map = it->second;

		for (auto jt = map.begin(); jt != map.end(); ++jt) {

			const std::vector<Entity>& v = jt->second;

			for (auto kt = v.begin(); kt != v.end(); ++kt) {
				final += "\t\t\t{\n";

				// Get current entity
				const Entity& e = *kt;

				// Go through each possible tags and see if the entity has it 
				TagMap tagMap{};	// create tagmap

				for (auto it2 = tagMap.Begin(); it2 != tagMap.End(); ++it2) {
					if (e.HasTag(it2->second)) {
						final += "\t\t\ttag=" + it2->second + "\n";
					}
				}

				// Let's add the components
				final += "\n\t\t\t\tcomponents = {\n";

				// Let's go through each components
				/* Transform Component check */
				const float scale = 2; /* JAKE TODO SCALE IS HARDCODED TO 2.0. Must fix this when dealing with scale later*/

				if (e.HasComponent<TransformComponent>()) {
					const TransformComponent& tC = e.GetComponent<TransformComponent>();
					final += "\t\t\t\t\ttransform = {\n";
					final += "\t\t\t\t\t\tposition = { x = " + std::to_string((tC.position.x - worldDisplacement.x) * scale) + ", y= " + std::to_string((tC.position.y - worldDisplacement.y) * scale) + " },\n";
					final += "\t\t\t\t\t\tscale = { x = " + std::to_string(scale) + ", y= " + std::to_string(scale) + " },\n";
					final += "\t\t\t\t\t\trotation = " + std::to_string(tC.rotation) + "\n";
					final += "\t\t\t\t\t},\n";
					// If not final component, then add comma
				}

				/* Sprite Component */
				if (e.HasComponent<SpriteComponent>()) {
					const SpriteComponent& sC = e.GetComponent<SpriteComponent>();
					final += "\t\t\t\t\tsprite = {\n";
					final += "\t\t\t\t\t\ttexture_asset_id = \"" + sC.assetId + "\"" + ",\n";
					final += "\t\t\t\t\t\twidth = " + std::to_string(sC.width) + ",\n";
					final += "\t\t\t\t\t\theight = " + std::to_string(sC.height) + ",\n";
					final += "\t\t\t\t\t\tz_index = " + std::to_string(sC.zIndex) + "\n";
					final += "\t\t\t\t\t},\n";
				}


				/* Rigid body Component*/
				if (e.HasComponent<RigidBodyComponent>()) {
					const RigidBodyComponent& rBC = e.GetComponent<RigidBodyComponent>();
					final += "\t\t\t\t\trigidbody = {\n";
					final += "\t\t\t\t\t\tvelocity = { x = " + std::to_string(rBC.velocity.x) + ", y = " + std::to_string(rBC.velocity.y) + " }\n";
					final += "\t\t\t\t\t},\n";
				}

				/* Box Collider Component */
				if (e.HasComponent<BoxColliderComponent>()) {
					const BoxColliderComponent& bCC = e.GetComponent<BoxColliderComponent>();

					final += "\t\t\t\tboxcollider = {\n";
					final += "\t\t\t\t\twidth = " + std::to_string(bCC.width) + ",\n";
					final += "\t\t\t\t\theight = " + std::to_string(bCC.height) + ",\n";

					final += "\t\t\t\t\toffset = { x = " + std::to_string(bCC.offset.x * scale) +
						", y = " + std::to_string(bCC.offset.y * scale) + " }\n";

					final += "\t\t\t\t},\n";

				}


				/* Player Controlled Component i.e the CAMERA */
				if (e.HasComponent<PlayerControlledComponent>()) {
					const PlayerControlledComponent& pCC = e.GetComponent<PlayerControlledComponent>();
					final += "\t\t\t\tplayer_controlled = {\n";
					final += "\t\t\t\t},\n";
				}


				if (e.HasComponent<KeyboardControlledComponent>()) {
					const KeyboardControlledComponent& kCC = e.GetComponent<KeyboardControlledComponent>();
					const glm::vec2& upVelocity = kCC.upVelocity;
					const glm::vec2& rightVelocity = kCC.rightVelocity;
					const glm::vec2& downVelocity = kCC.downVelocity;
					const glm::vec2& leftVelocity = kCC.leftVelocity;

					final += "\t\t\t\tkeyboard_controller = {\n";
					final += "\t\t\t\t\tup_velocity = { x = " + std::to_string(upVelocity.x) + ", y = " + std::to_string(upVelocity.y) + " },\n";
					final += "\t\t\t\t\tright_velocity = { x = " + std::to_string(rightVelocity.x) + ", y = " + std::to_string(rightVelocity.y) + " },\n";
					final += "\t\t\t\t\tleft_velocity = { x = " + std::to_string(leftVelocity.x) + ", y = " + std::to_string(leftVelocity.y) + " },\n";
					final += "\t\t\t\t\tdown_velocity = { x = " + std::to_string(downVelocity.x) + ", y = " + std::to_string(downVelocity.y) + " }\n";
					final += "\t\t\t\t},\n";	// end of player controlled
				}

				final += "\t\t\t\t}\n"; // end of components;


				final += "\t\t\t},\n"; // end of entity with comma

			}
		}
	}
	// do player 
	//final += "\t\t},\n";	// JAKE - add entities in here later.
	final += "\t}\n"; // close entities
	final += "}"; // close level 

	Logger::Log("FINAL : " + final + " END");

	// Lua Path
	std::string s3 = R"(C:\Users\17329\source\repos\2dgameengine-2023\2dgameengine-2023\assets\scripts\test.lua)";
	std::ofstream luaOutputFile;
	luaOutputFile.open(s3, std::ios::trunc);
	if (luaOutputFile.is_open()) {
		luaOutputFile << final;
	}
	else {
		Logger::Err("Lua file could not be generated");
	}
	luaOutputFile.close();




}

void WorldEditor::RenderSelectWorldDimensionsWindow(TileMap& tileMap, bool& generateWorld) {
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize;
	if (generateWorld == false && ImGui::Begin("Tilemap Properties", NULL, windowFlags)) {
		const char* fileNames[2] = { "jungle.png", "jungle-night.png" };

		static int selectedSpriteIndex = 0;

		// Create an instance of ImGuiFileDialog
		//ImGui::Combo("texture id", &selectedSpriteIndex, fileNames, IM_ARRAYSIZE(fileNames));

		// num rows
		ImGui::InputInt("Number of Rows:", &tileMap.numRows);
		// new cols
		ImGui::InputInt("Number of Columns:", &tileMap.numCols);
		// tile size 
		ImGui::InputInt("Tile Size (default 32)", &tileMap.tileSize);
		// scale 
		ImGui::InputDouble("Scale (default 2.0)", &tileMap.scale);

		if (ImGui::Button("Create World")) {
			generateWorld = true;

		}
		ImGui::End();
	}
}

void WorldEditor::GenerateGrid(TileMap& tileMap, SDL_Rect& camera) {

	// Render Each World Grid

		for (int i = 0; i < tileMap.numRows; i++)
		{
			for (int j = 0; j < tileMap.numCols; j++)
			{
				// Create a checkerboard
				if ((j - i) % 2 == 0 && (i - j) % 2 == 0)
					SDL_SetRenderDrawColor(renderer, 125, 125, 125, 70);
				else
					SDL_SetRenderDrawColor(renderer, 200, 200, 200, 70);
				// Can have a zoom variable to handle zoom later. multile it against (tileMap.tileSize * zoom)
				// SDL_Rect newRect = { (std::floor(j * tileMap.tileSize * zoom)) - camera.x, (std::floor(i * mTileSize * zoom)) - camera.y, std::ceil(mTileSize * zoom), std::ceil(mTileSize * zoom) };

				SDL_Rect newRect = { (std::floor(j * tileMap.tileSize)) + worldDisplacement.x - camera.x, (std::floor(i * tileMap.tileSize)) + worldDisplacement.y - camera.y, std::ceil(tileMap.tileSize), std::ceil(tileMap.tileSize) };

				SDL_RenderFillRect(renderer, &newRect);
			}
		}
	
}

void WorldEditor::OnRightClick(RightMouseClickedEvent& event) {

	if (selectedTileWindowProperties.isSelected == true) {
		selectedTileWindowProperties.isSelected = false;
	}

	/* Clear mouse selected tile */
	if (mouseSelectedTile.entity != NULL) {
		// Kill the entity
		mouseSelectedTile.entity.Kill();
		mouseSelectedTile.entity = NULL;
		mouseSelectedTile.tag = Tag::None;
		mouseSelectedTile.groups.clear();
	}
	// Remove an entity from where it is placed on the map
	else if (mouseSelectedTile.entity == NULL) {

		// Let's check for the ordered entities first before we try to remove a tile
		// we must first get the x and y of the click
		const int& x = event.GetX() - (tileMap.tileSize / 2);
		const int& y = event.GetY() - (tileMap.tileSize / 2);
		DeleteOrderedEntity(x, y);

	
		/* Lets go through tiles and find the one underneath that we want to get rid of */
		// Gotta break it down into 
		/* Transform x and y of mouse into x and y indices on map */
		const int& a = ceil((x - worldDisplacement.x) / tileMap.tileSize);			// This will get the exact index
		const int& b = ceil((y - worldDisplacement.y) / tileMap.tileSize);

		if (worldMap->HasTile(a, b)) {
			worldMap->RemoveTile(Vec2{a,b});
		};


	

		
	}

	


}

void WorldEditor::DeleteOrderedEntity(int x, int y) {

	/* Jake Could make logic branch to try to get pixel perfect click, but meh */

	// this is the index
	int closestValueX = std::numeric_limits<int>::max();	// fill up to max, not infinity because memory inefficient.. leaves open to edge case bugs though


	for (auto it = orderedEntities.Begin(); it != orderedEntities.End(); ++it) {
		// if not, then we need to see if it's the closest value
		if (abs(it->first - x) <= abs(closestValueX - x)) {
			closestValueX = it->first;
		}
	}

	// Needs to be within 8 pixels of the center
	if (abs(closestValueX - x) > 10) {
		return;
	}

	int closestValueY = std::numeric_limits<int>::max();

	try {
		//  the next map for y, if its within a certain range, maybe 12 pixels
		const std::map<int, std::vector<Entity>>& m = orderedEntities.At(closestValueX);

		for (auto it = m.begin(); it != m.end(); ++it) {
			if (abs(it->first - y) <= abs(closestValueY - y)) {
				closestValueY = it->first;
			}
		}

	}
	catch (...) {
		Logger::Err("I should write safe code.. so this block exists. lol. Maybe it'll actually be worth it? ");
	}

	// needs to be within 8 pixels of the center
	if (abs(closestValueY - y) > 10) {
		return;
	}

	// Private scope so I can reuse the interator it lol
	{
		/* Jake todo, we can sort by z index later, perhaps grab by Z */
		auto it = orderedEntities.Find(closestValueX);

		if (it != orderedEntities.End()) {
			std::map<int, std::vector<Entity>>& m = it->second;

			auto jt = m.find(closestValueY);

			if (jt != m.end()) {
				std::vector<Entity>& v = jt->second;

				Logger::Log("Size of vector before: " + std::to_string(v.size()) + "\n");

				if (v.size() > 0) {
					Entity& e = v.back();
					v.pop_back();
					Logger::Log("Size of vector after: " + std::to_string(v.size()) + "\n");

					e.Kill();

					// Lets delete the vector entirely
					if (v.size() == 0) {
						m.erase(jt);	// deletes the key entirely from the y
						if (it->second.size() == 0) {
							orderedEntities.Erase(it); // deletes the key entirely from root map x
						}
					}
					return;	// jump out of this because we deleted something

				}
				else {
					Logger::Err(
						"Somehow you managed to f this up -> " + std::to_string(v.size()) +
						"the vector of entities had a size that was 0 or less.. "
					);
				}
			}
		}

	}


}