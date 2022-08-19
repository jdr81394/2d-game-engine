#ifndef WORLDEDITOR_H
#define WORLDEDITOR_H
#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"
#include <sol/sol.hpp>
#include <memory>
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/MouseControlledComponent.h"
#include <stdio.h>
#include <string>
#include <filesystem>

#include <glm/glm.hpp>

class WorldEditor {

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        bool isRunning;
        int windowWidth;
        int windowHeight;
        int millisecsPreviousFrame = 0;
        SDL_Rect camera;

        sol::state &  lua;
        
        const std::unique_ptr<Registry>&  registry;
        AssetStore * assetStore; 
        std::unique_ptr<EventBus>& eventBus;

    public:
        WorldEditor(
            sol::state &  lua, 
            const std::unique_ptr<Registry>& registry, 
            std::unique_ptr<EventBus>& eventBus
        );
        ~WorldEditor();
        
        SDL_Window* GetWindow();
        AssetStore * GetAssetStore();
        SDL_Renderer * GetRenderer();
        void Initialize();
        void Run();
        void Update();
        void Render();
        void Destroy();

};

#endif