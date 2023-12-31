#ifndef GAME_H
#define GAME_H
#include "../EventBus/EventBus.h"
#include "../ECS/ECS.h"
#include <SDL.h>
#include "../AssetStore/AssetStore.h"
#include "../WorldEditor/WorldEditor.h"
#include "../Events/CollisionEvent.h"
#include <sol/sol.hpp>

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
    private:
        bool isRunning;
        bool isDebug;
        bool isWorldEditor;
        int millisecsPreviousFrame = 0;
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Rect camera;
        WorldEditor * worldEditor;

        sol::state lua;
        
        std::unique_ptr<Registry> registry;
        std::unique_ptr<AssetStore> assetStore; 
        std::unique_ptr<EventBus> eventBus;
    public:
        Game();
        ~Game();
        void PrepAssetStoreForWorldEditor();
        void Initialize();
        void Run();
        void Setup();
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();

        static int windowWidth;
        static int windowHeight;
        static int mapWidth;
        static int mapHeight;
};

#endif
