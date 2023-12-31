#ifndef WORLDEDITORSYSTEM_H
#define WORLDEDITORSYSTEM_H
#include "../ECS/ECS.h"
#include "../Events/LeftMouseHeldDownEvent.h"
#include "../Events/LeftMouseClickedEvent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <iostream>
#include <fstream>

#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>


#include <string>

struct TileMap { std::string mapFilePath; int numRows; int numCols; int tileSize; double scale; };

class WorldMapDataStructure {

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

	void LogTiles() {
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

	WorldMapDataStructure() = default;

	WorldMapDataStructure(TileMap tileMap) {
	/*	Logger::Err("HEWorldMapDataStructureRE MF");
		Logger::Log("numRows: " + std::to_string(tileMap.numRows));
		Logger::Log("numCols: " + std::to_string(tileMap.numCols));*/

		Logger::Log("IN WORLD MAP");
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

	std::set<std::string> GetAllUniqueAssetIds() {
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

	void AddTile(Entity e) {
		const TransformComponent tC = e.GetComponent<TransformComponent>();
		const SpriteComponent sC = e.GetComponent<SpriteComponent>();

		int eX = ceil(tC.position.x / sC.width);			// This will get the exact index
		int eY = ceil(tC.position.y / sC.height);
		
		Logger::Log("SPRITE COMPONENT HELD DOWN 1: " + sC.assetId);

		// JAKE - upon initialization I should initialize each key that the map should have. That out of range error should never occur.

		// Check if entity exists at that place already
		// Go through the maps at the specific x and y value keys




		try {
			std::map<int, Entity> &map2 = tilesOrderedByX.at(eX);
			
			//for (auto it = map2.begin(); it != map2.end(); ++it) {
			//	//std::cout << "Value at key " << it->first << ": " << it->second->GetId() << std::endl;
			//}
			
			Entity e2 = map2.at(eY);

			if (e2 != NULL && e2.HasComponent<SpriteComponent>()) {
				const SpriteComponent sC2 = e2.GetComponent<SpriteComponent>();



				Logger::Log("SPRITE COMPONENT HELD DOWN 2: " + sC2.assetId);
			}



			if(e != NULL && e2 == NULL)
				Logger::Log("ID1: " + std::to_string(e.GetId()) + "  ID2: " + std::to_string(e2.GetId()));
			
			if (e2 != NULL) {
				
				// Delete element
				map2.erase(eY);
				//Logger::Log("TILES ORDERED BY X: entity with this id was deleted: " + std::to_string( e2->GetId() ) + ", new entity: " + std::to_string(e->GetId()));
				e2.Kill();
			}
		}
		catch (const std::out_of_range& e) {
			Logger::Err("Key not found in ordered by X: ");
		}

		try {
			std::map<int, Entity> &map2 = tilesOrderedByY.at(eY);
			Entity e2 = map2.at(eX);
			if (e2 != NULL) {
				map2.erase(eX);
				//Logger::Log("TILES ORDERED BY Y: entity with this id was deleted: " + std::to_string(e2->GetId()) + ", new entity: " + std::to_string(e->GetId()));
				e2.Kill();
			}

		}
		catch (const std::out_of_range& e) {
			Logger::Err("Key not found in ordered by Y: ");
		}



		// Add the entity, go to the vector 
		tilesOrderedByX[eX][eY] = e;
		tilesOrderedByY[eY][eX] = e;

		//Logger::Log("id: " + std::to_string(tilesOrderedByX[eX][eY].GetId()));

	}


};

class WorldEditorSystem : public System {
private:
	Entity mouseSelectedTile;
	Registry* registry;
	TileMap tileMap = {};
	WorldMapDataStructure * worldMap;
	bool generateWorld;


public:

	//MouseSelectedTexture mouseSelectedTexture; /* DEPRECATED */

	WorldEditorSystem(Registry *r) {
		this->registry = r;
		//this->mouseSelectedTexture = MouseSelectedTexture(); /* DEPRECATED */
		this->mouseSelectedTile = NULL;
		this->tileMap.tileSize = 32;
		this->tileMap.scale = 2.0;
		this->generateWorld = false;
		this->worldMap = nullptr;
	}




	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		//eventBus->SubscribeToEvent<LeftMouseClickedEvent>(this, &WorldEditorSystem::OnLeftClick);
		eventBus->SubscribeToEvent<LeftMouseHeldDownEvent>(this, &WorldEditorSystem::OnLeftMouseHeldDown);

	};


	void OnLeftMouseHeldDown(LeftMouseHeldDownEvent& event) {
		if (mouseSelectedTile != NULL) {

			int x = event.GetX();
			int y = event.GetY();

			if (x > tileMap.numCols * tileMap.tileSize || x <= 0) return;
			if (y > tileMap.numRows * tileMap.tileSize || y <= 0) return;

			Entity entity = registry->CreateEntity();

			/* For position, let's make it snap into place */
			const SpriteComponent sC = mouseSelectedTile.GetComponent<SpriteComponent>();
			const TransformComponent tC = mouseSelectedTile.GetComponent<TransformComponent>();

			Logger::Log("SPRITE COMPONENT HELD DOWN: " + sC.assetId);

			int snapX = ceil(x / sC.width) * sC.width;
			int snapY = ceil(y / sC.height) * sC.height;

			if (snapX > tileMap.numCols * tileMap.tileSize - 10 ) return;
			if (snapY > tileMap.numRows * tileMap.tileSize - 10) return;


			glm::vec2 position = glm::vec2(snapX, snapY);
			entity.AddComponent<TransformComponent>(position);
			entity.AddComponent<SpriteComponent>(sC.assetId, 32, 32, 1);



			worldMap->AddTile(entity);
		
		}
	}

	void OnLeftClick(LeftMouseClickedEvent& event) {
		
		if (mouseSelectedTile != NULL) {
			int x = event.GetX();
			int y = event.GetY();


			if (x > tileMap.numCols * tileMap.tileSize || x <= 0) return;
			if (y > tileMap.numRows * tileMap.tileSize || y <= 0) return;



			Entity entity = registry->CreateEntity();

			/* For position, let's make it snap into place */
			const SpriteComponent sC = mouseSelectedTile.GetComponent<SpriteComponent>();


			int snapX = ceil(x / sC.width) * sC.width;
			int snapY = ceil(y / sC.height) * sC.height;

			if (snapX > tileMap.numCols * tileMap.tileSize - 10) return;
			if (snapY > tileMap.numRows * tileMap.tileSize - 10) return;

			glm::vec2 position = glm::vec2(snapX, snapY);
			entity.AddComponent<TransformComponent>(position);


			entity.AddComponent<SpriteComponent>(sC.assetId, 32, 32, 1);
			Logger::Log("SPRITE COMPONENT CLICK DOWN: " + sC.assetId);

			Logger::Log("Entity from mouse click : " + std::to_string(entity.GetId()));

			worldMap->AddTile(entity);

		}


	}


	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect& camera, SDL_Window* window) {
		ImGui::NewFrame();
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize;



		// Display a window for the tilemap:
		if (generateWorld == false && ImGui::Begin("Tilemap Properties", NULL, windowFlags)) {

			RenderSelectWorldDimensionsWindow(tileMap, generateWorld);


			ImGui::End();
		}

		if (generateWorld) {
			if(worldMap == nullptr) worldMap = new WorldMapDataStructure(tileMap);

			// Generate Grid for dropping tiles onto
			GenerateGrid(renderer, tileMap, camera);
			

			// Load Textures into Selection Pane
			auto allTextures = assetStore->GetAllTextures(); //  type std::map<std::string, SDL_Texture*>
			const int numCols = 5;
			int currentCount = 0;
			const ImVec2 spacing(0.0f, 5.0f);

			if (ImGui::Button("Log out World Map")) {
				worldMap->LogTiles();
				
			}


			ImGui::NewLine(); // Put space between Button and the rest of the tiles

			if (ImGui::Button("Create Map")) {
				GenerateFinalWorldMap(renderer, assetStore, window);
			}

			ImGui::NewLine(); // Put space between Button and the rest of the tiles


			for (auto it = allTextures.begin(); it != allTextures.end(); ++it) {



				
				ImTextureID textureId = it->second;

				ImVec2 size = ImVec2(tileMap.tileSize, tileMap.tileSize); // ImVec2& - size - width and height
				ImVec2 uv0; // default ImVec2& - top left position - top left corner of the image 
				ImVec2 uv1; // default ImVec2& -bottom right position - bottom right corner of the image
				int framePadding = 0;	// idk what this does
				//imVec4& - tint color rgba values
				ImVec4 uv4Tint;
				// ImVec4& - border color rgba values
				ImVec4 uv4Border;
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

				if (ImGui::ImageButton(textureId, size)) {
					if (mouseSelectedTile != NULL) {
						registry->KillEntity(mouseSelectedTile);
						mouseSelectedTile = NULL;
					}
					mouseSelectedTile = registry->CreateEntity();
				};

				if (mouseSelectedTile != NULL) {
					
					/* Putting this here kind of fixes the bug with the first click. Doesn't create anything but prevents it from showing up */
					if (!mouseSelectedTile.HasComponent<TransformComponent>()) {
						glm::vec2 position = glm::vec2(mX, mY);
						mouseSelectedTile.AddComponent<TransformComponent>(position);
					}

					if (!mouseSelectedTile.HasComponent<SpriteComponent>()) {
						mouseSelectedTile.AddComponent<SpriteComponent>(it->first, 32, 32, 100);
						Logger::Log("asset id: " + it->first);
					}
					/* ---------------------------------------------------------------------------------------------------  */


					// Update it's position based on the users mouse
					TransformComponent& tc = mouseSelectedTile.GetComponent<TransformComponent>();
					tc.position.x = mX;
					tc.position.y = mY;

				}


				
			}




			// Generate the world 
			ImGui::End();
		}



		ImGui::Render();
		ImGuiSDL::Render(ImGui::GetDrawData());
	}

private:

	void GenerateFinalWorldMap(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Window* window) {
	
		// Keep track all the images that have been used
		std::set<std::string> allAssetIds;
		std::vector<SDL_Texture*> allTextures;

		// 1. get all the assetIds
		allAssetIds = worldMap->GetAllUniqueAssetIds();


		// 2. then use the asset ids to fetch all the textures, when i am doing this
		for (auto it = allAssetIds.begin(); it != allAssetIds.end(); ++it) {
			SDL_Texture* newTexture = assetStore->CopyTexture(renderer,assetStore->GetTexture(*it), window);
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
								if (count <= 10) {
									x = count;
									y = 0;
								}
								else {
									x = count % 10;		// remainder
									y = floor(count / 10); 

								}

								/* 4d step: */

								if (std::next(it2) != map2.end() ) {
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

		delete pixels;

		for (const auto& texture : allTextures) {
			SDL_DestroyTexture(texture);
		}

		SDL_SetRenderTarget(renderer, NULL);



		
	}

	void RenderSelectWorldDimensionsWindow(TileMap& tileMap, bool& generateWorld) {
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
			Logger::Log("HEREHEREHERE");
			generateWorld = true;

		}
	}

	void GenerateGrid(SDL_Renderer* renderer, TileMap& tileMap, SDL_Rect& camera) {
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize;

		// Render Each World Grid
		if (ImGui::Begin("Tile Selection", NULL, windowFlags)) {

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

					SDL_Rect newRect = { (std::floor(j * tileMap.tileSize)) - camera.x, (std::floor(i * tileMap.tileSize)) - camera.y, std::ceil(tileMap.tileSize), std::ceil(tileMap.tileSize) };

					SDL_RenderFillRect(renderer, &newRect);
				}
			}
		}
	}

};

#endif


