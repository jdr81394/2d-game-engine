#include "Game.h"
#include "./LevelLoader.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Systems/ProjectileLifecycleSystem.h"
#include "../Systems/RenderHealthSystem.h"
#include "../Systems/RenderHealthTextSystem.h"
#include "../Systems/RenderGUISystem.h"
#include "../Systems/ScriptSystem.h"
#include "../Systems/WorldEditorSystem.h"
#include "../Events/LeftMouseClickedEvent.h"
#include "../Events/RightMouseClickedEvent.h"

#include <SDL_image.h>
#include <SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <imgui/imgui_impl_sdl.h>
#include <glm/glm.hpp>
#include <iostream>
#include <filesystem> // C++17 or later
#include <fstream>

int Game::windowHeight;
int Game::windowWidth;
int Game::mapHeight;
int Game::mapWidth;

Game::Game() {
    isRunning = false;
    isDebug = false;
    isWorldEditor = true;
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

    // Must be initialized to use anything TTF related.
    if(TTF_Init() != 0) {
        Logger::Err("Error initializing SDL TTF");
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
        //SDL_WINDOW_BORDERLESS
        SDL_WINDOW_RESIZABLE
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

    // Initialize the ImGui Context 
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, windowWidth, windowHeight);

    // Initialize the camera view with the entire screen area
    camera.x = 0;
    camera.y = 0;
    camera.w = windowWidth;
    camera.h = windowHeight;
}

void Game::ProcessInput() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {

        // ImGui SDL input
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        ImGuiIO & io = ImGui::GetIO();
        int mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);


        if (io.MouseDown[0]) {
            //Logger::Log("MOUSE DOWN : " + std::to_string(mouseX) + " Y : " + std::to_string(mouseY));
            eventBus->EmitEvent<LeftMouseHeldDownEvent>(mouseX, mouseY);
        }



        // SDL function that gets the mouse state and puts it in these variables

        // ImGUI has it's own system for keeping track of the mouse position
        // The MousePos property keeps track of it and it takes an ImVec2() type
        io.MousePos = ImVec2(mouseX, mouseY);

        // Bitwise operation to tell us what mouse is down. 
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

     


        
        // Handle core SDL events (close window, key pressed, etc.)
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
            case SDL_MOUSEBUTTONDOWN:
                if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
                    eventBus->EmitEvent<LeftMouseClickedEvent>(mouseX, mouseY);
            

                }
                else if (sdlEvent.button.button == SDL_BUTTON_RIGHT) {
                    eventBus->EmitEvent<RightMouseClickedEvent>(mouseX, mouseY);

                }
                break;

        }
    }
}


void Game::Setup() {
    // Add the systems that need to be processed in our game
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<RenderColliderSystem>();
    registry->AddSystem<DamageSystem>();
    registry->AddSystem<KeyboardControlSystem>();
    registry->AddSystem<CameraMovementSystem>();
    registry->AddSystem<ProjectileEmitSystem>();
    registry->AddSystem<ProjectileLifecycleSystem>();
    registry->AddSystem<RenderTextSystem>();
    registry->AddSystem<RenderHealthSystem>();
    registry->AddSystem<RenderHealthTextSystem>();
    registry->AddSystem<RenderGUISystem>();
    registry->AddSystem<ScriptSystem>();


    // Create the bindings between C++ and Lua
    registry->GetSystem<ScriptSystem>().CreateLuaBindings(lua);
    
    // Determine if it is map editor or not
   

    if (isWorldEditor == false) {
        // Load 1st level
        LevelLoader loader;
        lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
        loader.LoadLevel(lua, registry, assetStore, renderer, 1);
    }
    else {
        // Do world editor stuff
        registry->AddSystem<WorldEditorSystem>(registry.get()); // THIS GETS RAW POINTER, if the unique pointer is destroyed for whatever reason this will be dangling
        // Determine the tilemap first
        PrepAssetStoreForWorldEditor();
   

    }

}


void Game::PrepAssetStoreForWorldEditor() {

    std::string directoryPath = "./assets/images";
    std::vector<std::string> fileNames;

    // Get file name
    // DONT NEED ACTUALLY Get the asset type "font" or "texture" (will be texture for us")
    // Get the asset id - name of png + -texture without file extension
    
    
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg") {
            // Store the filename in a string variable
            //fileNames.push_back(entry.path().filename().string());
            assetStore->AddTexture(renderer, entry.path().stem().string() + "-texture", entry.path().generic_string());
            //Logger::Log(entry.path().filename().string());
        }
    }

    std::fstream mapFile;


    // Create each tile on selector
    std::string fullMapPaths = "./assets/tilemaps";
    std::vector<std::string> mapFileNames;

    for (const auto& entry : std::filesystem::directory_iterator(fullMapPaths)) {
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg") {
            // Store the filename in a string variable
            mapFileNames.push_back(entry.path().generic_string());
            //Logger::Log("File name generic string: " + entry.path().generic_string());
        }
    }

 

    // each filename will be a whole file
    for (auto it = mapFileNames.begin(); it != mapFileNames.end(); ++it) {
        
        std::string mapFilePath = *it; // file like jungle.png or jungle-night.png

        // get each tile and put it into the selector 
        int mapNumRows = 3;	
        int mapNumCols = 10;
        int tileSize = 32;
        for (int y = 0; y < mapNumRows; y++) {
            for (int x = 0; x < mapNumCols; x++) {
                SDL_Rect portionRect = { x * tileSize, y * tileSize, tileSize, tileSize };
                SDL_Surface* portionSurface = SDL_CreateRGBSurface(0, portionRect.w, portionRect.h, 32, 0, 0, 0, 0);
                SDL_Surface* fullImage = IMG_Load(mapFilePath.c_str());

                SDL_BlitSurface(fullImage, &portionRect, portionSurface, nullptr);

                // Create a texture from the portion surface
                SDL_Texture* portionTexture = SDL_CreateTextureFromSurface(renderer, portionSurface);

                const std::string assetId = mapFilePath + std::to_string(x) + "-" + std::to_string(y) + "-texture";

                assetStore->AddTextureDirectly(assetId, portionTexture);

                SDL_FreeSurface(portionSurface);

            }
        }
    }

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
    registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<MovementSystem>().SubscribeToEvents(eventBus);
    if (isWorldEditor) registry->GetSystem<WorldEditorSystem>().SubscribeToEvents(eventBus);

    // Update the registry to process the entities that are waiting to be created/deleted
    registry->Update();
    
    // Ask all the systems to update
    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update(eventBus);
    registry->GetSystem<CameraMovementSystem>().Update(camera);
    registry->GetSystem<ProjectileEmitSystem>().Update(registry);
    registry->GetSystem<ProjectileLifecycleSystem>().Update();
    registry->GetSystem<ScriptSystem>().Update(deltaTime, SDL_GetTicks());
}    


void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);


    if (isWorldEditor) {
        registry->GetSystem<WorldEditorSystem>().Update(renderer, assetStore, camera, window);
    }
    // TODO: Render game objects...
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore, camera);
    registry->GetSystem<RenderTextSystem>().Update(renderer, assetStore, camera);
    registry->GetSystem<RenderHealthSystem>().Update(renderer, camera);
    registry->GetSystem<RenderHealthTextSystem>().Update(renderer, assetStore, camera);

    if (isDebug) {
        registry->GetSystem<RenderColliderSystem>().Update(renderer, camera);
        if (isWorldEditor == false) {
            registry->GetSystem<RenderGUISystem>().Update(registry, camera);
        }
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
    ImGuiSDL::Deinitialize();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
