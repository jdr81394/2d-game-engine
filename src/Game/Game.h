#ifndef GAME_H
#define GAME_H

#include "../ECS/ECS.h"
#include <SDL2/SDL.h>
#include "../AssetStore/AssetStore.h"
#include "../Events/CollisionEvent.h"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
    private:
        bool isRunning;
        bool isDebug;
        int millisecsPreviousFrame = 0;
        SDL_Window* window;
        SDL_Renderer* renderer;

        std::unique_ptr<Registry> registry;
        std::unique_ptr<AssetStore> assetStore; 
        std::unique_ptr<EventBus> eventBus;
    public:
        Game();
        ~Game();
        void Initialize();
        void Run();
        void Setup();
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
        void LoadLevel(int level);

        int windowWidth;
        int windowHeight;
};

#endif
