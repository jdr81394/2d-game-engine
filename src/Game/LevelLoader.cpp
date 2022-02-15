#include "LevelLoader.h"
#include "Game.h"
#include <fstream>
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"
#include <sol/sol.hpp>
#include <string>
#include "../Logger/Logger.h"

LevelLoader::LevelLoader() {
    Logger::Log("Level Loader constructor called!");
};

LevelLoader::~LevelLoader() {
    Logger::Log("Level Loader deconstructor called!");
};

void LevelLoader::LoadLevel(sol::state& lua, std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, int levelNumber) {
    
    // This isnt "interpreting the script". Its not executing the script. Its just loading the file.
    sol::load_result script = lua.load_file("./assets/scripts/Level"+std::to_string(levelNumber) + ".lua");

    // This checks the syntax of our script, but it does not execute the script.
    if(!script.valid()) {
        sol::error err  = script;
        std::string errorMessage = err.what();
        Logger::Err("Error loading hte lua script: " + errorMessage);
        return;
    }


    // Executes the script using the Sol (the lua) state
    lua.script_file("./assets/scripts/Level" + std::to_string(levelNumber) + ".lua");

    // Read the big table for the current level
    sol::table level = lua["Level"];

    //////////////////////////////////////////////////////////////////////////////////////////
    // Read the level assets
    //////////////////////////////////////////////////////////////////////////////////////////

    sol::table assets = level["assets"];

    int i = 0;
    while(true) {
        // The optional is just saying this only "maybe" exists
        sol::optional<sol::table> hasAsset = assets[i];

        // If the sol::table does not exist
        if(hasAsset == sol::nullopt) {
            break;
        }
        
        sol::table asset = assets[i];
        std::string assetType = asset["type"];
        std::string assetId = asset["id"];
        if(assetType == "texture") {
            assetStore->AddTexture(renderer, assetId, asset["file"]);
            Logger::Log("New texture asset loaded to the asset store, id: " + assetId);

        }

        if(assetType == "font") {
            assetStore->AddFont(assetId, asset["file"], asset["font_size"]);
            Logger::Log("New asset font added to the asset store, id: " + assetId);
        }

        i++;
    }

    Logger::Log("we just opened level 1 lua");


    //////////////////////////////////////////////////////////////////////////////////////////
    // Read the level tilemap information
    //////////////////////////////////////////////////////////////////////////////////////////
    // TODO:....

    //////////////////////////////////////////////////////////////////////////////////////////
    // Read the level entities and components
    //////////////////////////////////////////////////////////////////////////////////////////
    // TODO:....


    // // Add assets to the asset store
    // std::string tankImage = "tank-image";
    // std::string tankImagePath = "./assets/images/tank-panther-right.png";

    // std::string truckImage = "truck-image";
    // std::string truckImagePath = "./assets/images/truck-ford-right.png";

    // std::string chopperImage = "chopper-image";
    // std::string chopperImagePath = "./assets/images/chopper-spritesheet.png";

    // assetStore->AddTexture(renderer, tankImage, tankImagePath);
    // assetStore->AddTexture(renderer, truckImage, truckImagePath);
    // assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
    // assetStore->AddTexture(renderer, chopperImage, chopperImagePath);
    // assetStore->AddTexture(renderer, "radar-image","./assets/images/radar.png");
    // assetStore->AddTexture(renderer, "bullet-image", "./assets/images/bullet.png");
    // assetStore->AddTexture(renderer, "tree-image", "./assets/images/tree.png");
    // assetStore->AddFont("charriot-font", "./assets/fonts/charriot.ttf", 14);

    // // Load the tilemap
    // const int tileSize = 32; // This is the number of pixels each square is by length/width
    // double tileScale = 2.5;
    // int mapNumCols = 25;    // This is the number of columns each map has.
    // int mapNumRows = 20;

    // std::fstream mapFile;   // fstream is a library that allows you to read textfiles and Standard I/O
    // mapFile.open("./assets/tilemaps/jungle.map");    // This opens up the map and gets it ready to be read

    // for (int y = 0; y < mapNumRows; y++) {
    //     for (int x = 0; x < mapNumCols; x++) {
    //         // Opens up a blank character variable.. i guess to assign the text value to.
    //         char ch; 
            
    //         // The 2 digits represent the y and x axis respectively.
    //         // For example, "21" represents the water tile
    //         // If you look at the png, you must go down 2 * tileSize, and 1 * tileSize to the right to get it.
    //         mapFile.get(ch);                            // Gets the character from the stream. I guess this allows it to be moved along.
    //         int srcRectY = std::atoi(&ch) * tileSize;   // std::atoi is a character to integer conversion.

    //         mapFile.get(ch);
    //         int srcRectX = std::atoi(&ch) * tileSize;
            
    //         mapFile.ignore();                           // We are ignoring the ","

    //         Entity tile = registry->CreateEntity();
    //         tile.Group("tiles");
    //         // tile.AddComponent<TransformComponent>(
    //         //     glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), 
    //         //     glm::vec2(tileScale, tileSize), 0.0          // I put tileSize which is why it was erroring.
    //         // );
    //         tile.AddComponent<TransformComponent>(
    //             glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize) ), 
    //             glm::vec2(tileScale, tileScale), 0.0);              // 2nd value is scale, thats what is wrong in the uncommented.

    //         tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, false, srcRectX, srcRectY);
    //     }
    // }
    // mapFile.close();        // Close the stream

    // // Whenever loading tile map, as soon as were done loading tile map
    // // we want the map width to be loaded in the size of pixels
    // Game::mapWidth = mapNumCols * tileSize * tileScale;
    // Game::mapHeight = mapNumRows * tileSize * tileScale;
    // SDL_Color green = {0,255,0};

    // // Create an entity
    // Entity chopper = registry->CreateEntity();
    // Logger::Log("player just created");
    // chopper.Tag("player");
    // chopper.AddComponent<TransformComponent>(glm::vec2(10.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
    // chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    // chopper.AddComponent<SpriteComponent>(chopperImage, 32, 32, 2);
    // chopper.AddComponent<AnimationComponent>(2,30, true);
    // chopper.AddComponent<BoxColliderComponent>(32,32);
    // chopper.AddComponent<ProjectileEmitterComponent>(glm::vec2(500.0,500.0), 0, 10000, 20, true);
    // chopper.AddComponent<KeyboardControlledComponent>(glm::vec2(0, -200), glm::vec2(200, 0), glm::vec2(0, 200), glm::vec2(-200, 0));
    // chopper.AddComponent<CameraFollowComponent>();
    // chopper.AddComponent<HealthComponent>(100);
    // chopper.AddComponent<TextLabelComponent>(glm::vec2(0), "","charriot-font", green, true);       // Health label, and color will be changed in renderHealthTextSystem

    // Entity radar = registry->CreateEntity();
    // radar.AddComponent<TransformComponent>(glm::vec2(Game::windowWidth - 74, 10.0), glm::vec2(1.0, 1.0), 0.0);
    // radar.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    // radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 2, true);       // This image is 64 x 64 image
    // radar.AddComponent<AnimationComponent>(8,5, true);
   
    // Entity tank = registry->CreateEntity();
    // tank.Group("enemies");
    // tank.AddComponent<TransformComponent>(glm::vec2(1000.0, 1000.0), glm::vec2(1.0, 1.0), 45.0);
    // tank.AddComponent<RigidBodyComponent>(glm::vec2(60.0, 0.0));
    // tank.AddComponent<SpriteComponent>(tankImage, 32, 32, 2);
    // tank.AddComponent<BoxColliderComponent>(32,32);
    // tank.AddComponent<ProjectileEmitterComponent>(glm::vec2(100.0, 0.0), 2000, 3000, 20, false); // change to 10
    // tank.AddComponent<HealthComponent>(100);
    // tank.AddComponent<TextLabelComponent>(glm::vec2(0), "", "charriot-font", green, true);          // Health label, and color will be changed in renderHealthTextSystem
    // tank.Group("enemies");

    // Entity truck = registry->CreateEntity();
    // truck.Group("enemies");
    // truck.AddComponent<TransformComponent>(glm::vec2(1050.0, 750.0), glm::vec2(1.0, 1.0), 0.0);
    // truck.AddComponent<RigidBodyComponent>(glm::vec2(60.0, 0.0));
    // truck.AddComponent<SpriteComponent>(truckImage, 32, 32, 10);
    // truck.AddComponent<BoxColliderComponent>(32,32);
    // truck.AddComponent<ProjectileEmitterComponent>(glm::vec2(0.0, 100.0), 2000, 3000, 20, false); // change 100 to 10
    // truck.AddComponent<HealthComponent>(100);
    // truck.AddComponent<TextLabelComponent>(glm::vec2(0), "", "charriot-font", green, true);          // Health label, and color will be changed in renderHealthTextSystem
    // tank.Group("enemies");

    // Entity treeA = registry->CreateEntity();
    // treeA.Group("obstacles");
    // treeA.AddComponent<TransformComponent>(glm::vec2(1550.0, 750.0), glm::vec2(1.0, 1.0), 0.0);
    // treeA.AddComponent<SpriteComponent>("tree-image", 16, 32, 10);
    // treeA.AddComponent<BoxColliderComponent>(16,32);

    // Entity treeB = registry->CreateEntity();
    // treeB.Group("obstacles");
    // treeB.AddComponent<TransformComponent>(glm::vec2(1000.0, 750.0), glm::vec2(1.0,1.0), 0.0);
    // treeB.AddComponent<SpriteComponent>("tree-image", 16, 32, 10);
    // treeB.AddComponent<BoxColliderComponent>(16,32);
    
    // Entity label = registry->CreateEntity();
    // label.AddComponent<TextLabelComponent>(glm::vec2(Game::windowWidth / 2 - 40, 100), "This is a text label!!", "charriot-font", green, true);

}