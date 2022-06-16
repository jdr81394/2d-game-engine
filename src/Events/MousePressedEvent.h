#ifndef MOUSEPRESSEDEVENT_H
#define MOUSEPRESSEDEVENT_H

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"
#include "../EventBus/EventBus.h"
#include <sol/sol.hpp>

#include "../AssetStore/AssetStore.h"
#include "../WorldEditor/WorldEditor.h"
#include <SDL2/SDL.h>
#include <memory>

class MousePressedEvent : public Event {
        
    public:
        sol::state& lua;
        std::unique_ptr<Registry>& registry;
        std::unique_ptr<AssetStore>& assetStore;
        SDL_Renderer* renderer;
        std::unique_ptr<EventBus>& eventBus;
        std::unique_ptr<WorldEditor>& worldEditor;

        MousePressedEvent(      
            sol::state& lua, 
            std::unique_ptr<Registry>& registry, 
            std::unique_ptr<AssetStore>& assetStore, 
            SDL_Renderer* renderer,
            std::unique_ptr<EventBus>& eventBus,
            std::unique_ptr<WorldEditor>& worldEditor
        ) : lua(lua), registry(registry), assetStore(assetStore), renderer(renderer), eventBus(eventBus), worldEditor(worldEditor)  {}



};


#endif