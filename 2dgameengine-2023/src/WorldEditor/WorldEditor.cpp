#include "WorldEditor.h"


void WorldEditor::OnLeftMouseHeldDown(LeftMouseHeldDownEvent& event) {
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

		if (snapX > tileMap.numCols * tileMap.tileSize - 10) return;
		if (snapY > tileMap.numRows * tileMap.tileSize - 10) return;


		glm::vec2 position = glm::vec2(snapX, snapY);
		entity.AddComponent<TransformComponent>(position);
		entity.AddComponent<SpriteComponent>(sC.assetId, 32, 32, 1);



		worldMap->AddTile(entity);

	}
}

void WorldEditor::OnLeftClick(LeftMouseClickedEvent& event) {

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

void WorldEditor::Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect& camera, SDL_Window* window) {
	ImGui::NewFrame();
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize;



	// Display a window for the tilemap:
	if (generateWorld == false && ImGui::Begin("Tilemap Properties", NULL, windowFlags)) {

		RenderSelectWorldDimensionsWindow(tileMap, generateWorld);


		ImGui::End();
	}

	if (generateWorld) {
		if (worldMap == nullptr) worldMap = new OrderedTilesDataStructure(tileMap);

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


void WorldEditor::GenerateFinalWorldMap(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Window* window) {

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
							else {
								x = count % 9;		// remainder
								y = floor(count / 9);

							}

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

	delete pixels;

	for (const auto& texture : allTextures) {
		SDL_DestroyTexture(texture);
	}

	SDL_SetRenderTarget(renderer, NULL);




}

void WorldEditor::RenderSelectWorldDimensionsWindow(TileMap& tileMap, bool& generateWorld) {
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
}

void WorldEditor::GenerateGrid(SDL_Renderer* renderer, TileMap& tileMap, SDL_Rect& camera) {
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

