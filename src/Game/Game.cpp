#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>      // std::ifstream

Game::Game() {
    isRunning = false;
    isDebug = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
    eventBus = std::make_unique<EventBus>();
    Logger::Log("Game constructor called!");
}

Game::~Game() {
    Logger::Log("Game destructor called!");   
}

void Game::Initialize() {
    // Jake start at 12:05
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Err("Error initializing SDL.");
        return;
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = displayMode.w;
    windowHeight = displayMode.h;
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        Logger::Err("Error creating SDL window.");
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        Logger::Err("Error creating SDL renderer.");
        return;
    }
    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    isRunning = true;

    // Initialize the camera view with the entire screen area
    camera.x = 0;
    camera.y = 0;
    camera.w = windowWidth;
    camera.h = windowHeight;
}

void Game::ProcessInput() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
                if (sdlEvent.key.keysym.sym == SDLK_d) {
                    isDebug = !isDebug;
                }
                eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
                break;
        }
    }
}

void Game::LoadLevel(int level) {
   // Add the systems that need to be processed in our game
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<RenderColliderSystem>();
    registry->AddSystem<DamageSystem>();
    registry->AddSystem<KeyboardControlSystem>();

    // Add assets to the asset store
    std::string tankImage = "tank-image";
    std::string tankImagePath = "./assets/images/tank-panther-right.png";

    std::string truckImage = "truck-image";
    std::string truckImagePath = "./assets/images/truck-ford-right.png";

    std::string chopperImage = "chopper-image";
    std::string chopperImagePath = "./assets/images/chopper-spritesheet.png";

    assetStore->AddTexture(renderer, tankImage, tankImagePath);
    assetStore->AddTexture(renderer, truckImage, truckImagePath);
    assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
    assetStore->AddTexture(renderer, chopperImage, chopperImagePath);
    assetStore->AddTexture(renderer, "radar-image","./assets/images/radar.png");

    // Load the tilemap
    const int tileSize = 32; // This is the number of pixels each square is by length/width
    double tileScale = 2.5;
    int mapNumCols = 25;    // This is the number of columns each map has.
    int mapNumRows = 20;

    std::fstream mapFile;   // fstream is a library that allows you to read textfiles and Standard I/O
    mapFile.open("./assets/tilemaps/jungle.map");    // This opens up the map and gets it ready to be read

    for (int y = 0; y < mapNumRows; y++) {
        for (int x = 0; x < mapNumCols; x++) {
            // Opens up a blank character variable.. i guess to assign the text value to.
            char ch; 
            
            // The 2 digits represent the y and x axis respectively.
            // For example, "21" represents the water tile
            // If you look at the png, you must go down 2 * tileSize, and 1 * tileSize to the right to get it.
            mapFile.get(ch);                            // Gets the character from the stream. I guess this allows it to be moved along.
            int srcRectY = std::atoi(&ch) * tileSize;   // std::atoi is a character to integer conversion.

            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            
            mapFile.ignore();                           // We are ignoring the ","

            Entity tile = registry->CreateEntity();
            // tile.AddComponent<TransformComponent>(
            //     glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), 
            //     glm::vec2(tileScale, tileSize), 0.0          // I put tileSize which is why it was erroring.
            // );
            tile.AddComponent<TransformComponent>(
                glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize) ), 
                glm::vec2(tileScale, tileScale), 0.0);              // 2nd value is scale, thats what is wrong in the uncommented.

            tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, srcRectX, srcRectY);
        }
    }
    mapFile.close();        // Close the stream

    // Create an entity
    Entity chopper = registry->CreateEntity();
    chopper.AddComponent<TransformComponent>(glm::vec2(10.0, 100.0), glm::vec2(3.0, 3.0), 0.0);
    chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    chopper.AddComponent<SpriteComponent>(chopperImage, 32, 32, 2);
    chopper.AddComponent<AnimationComponent>(2,30, true);
    chopper.AddComponent<KeyboardControlledComponent>(glm::vec2(0, -80), glm::vec2(80, 0), glm::vec2(0, 80), glm::vec2(-80, 0));
    chopper.AddComponent<CameraFollowComponent>();

    Entity radar = registry->CreateEntity();
    radar.AddComponent<TransformComponent>(glm::vec2(windowWidth - 74, 10.0), glm::vec2(1.0, 1.0), 0.0);
    radar.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 2);       // This image is 64 x 64 image
    radar.AddComponent<AnimationComponent>(8,5, true);
   
    Entity tank = registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2(500.0, 10.0), glm::vec2(3.0, 3.0), 45.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(-30.0, 0.0));
    tank.AddComponent<SpriteComponent>(tankImage, 32, 32, 2);
    tank.AddComponent<BoxColliderComponent>(32,32, glm::vec2(0));

    Entity truck = registry->CreateEntity();
    truck.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(3.0, 3.0), 0.0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 0.0));
    truck.AddComponent<SpriteComponent>(truckImage, 32, 32, 10);
    truck.AddComponent<BoxColliderComponent>(32,32, glm::vec2(0));

}

void Game::Setup() {
    LoadLevel(1);
}

void Game::Update() {
    // If we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }

    // The difference in ticks since the last frame, converted to seconds
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

    // Store the "previous" frame time
    millisecsPreviousFrame = SDL_GetTicks();

    // Reset all event handlers for the current frame
    eventBus->Reset();

    // Perform the subscription of the events for all systems
    registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(eventBus);
    
    // Update the registry to process the entities that are waiting to be created/deleted
    registry->Update();
    
    // Ask all the systems to update
    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update(eventBus);
    registry->GetSystem<CameraMovementSystem>().Update(camera);
}    


void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    // TODO: Render game objects...
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore);
    if (isDebug) {
        registry->GetSystem<RenderColliderSystem>().Update(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::Run() {
    Setup();
    while (isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
