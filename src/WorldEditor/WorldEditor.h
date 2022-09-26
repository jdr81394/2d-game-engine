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
#include "../Systems/RenderSystem.h"
#include "../Events/ResizeWindowEvent.h"
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
        
        std::unique_ptr<Registry> registry;
        std::unique_ptr<AssetStore> assetStore; 
        std::unique_ptr<EventBus>& eventBus;

    public:
        WorldEditor(
            sol::state &  lua, 
            std::unique_ptr<EventBus>& eventBus
        );
        ~WorldEditor();
        std::unique_ptr<Registry> & GetRegistry();
        SDL_Window* GetWindow();
        SDL_Renderer * GetRenderer();
        void SetUp();
        void ArrangeAssetsOnWindow(ResizeWindowEvent & e );
        void Initialize();
        void Run();
        void Update();
        void Render();
        void Destroy();

};

#endif