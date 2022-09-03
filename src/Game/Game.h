#ifndef GAME_H
#define GAME_H
#include "../EventBus/EventBus.h"
#include "../ECS/ECS.h"
#include <SDL2/SDL.h>
#include "../AssetStore/AssetStore.h"
#include "../Events/CollisionEvent.h"
#include "../Events/ResizeWindowEvent.h"
#include "../Events/WorldEditorStartEvent.h"
#include "../WorldEditor/WorldEditor.h"
#include <sol/sol.hpp>
#include <thread>

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
    private:
        bool isRunning;
        bool isDebug;
        int millisecsPreviousFrame = 0;
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Rect camera;

        sol::state lua;
        
        std::unique_ptr<Registry> registry;
        std::unique_ptr<AssetStore> assetStore; 
        std::unique_ptr<EventBus> eventBus;

        std::unique_ptr<WorldEditor> worldEditor;
        
    public:
        Game();
        ~Game();
        void Initialize();
        void Run();
        void Setup();
        void InitializeWorldEditor(WorldEditorStartEvent& event);
        void MainWorldEditor();
        std::thread * worldEditorThread;

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
