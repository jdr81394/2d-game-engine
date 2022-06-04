#ifndef WORLDEDITORSTARTEVENT_H
#define WORLDEDITORSTARTEVENT_H

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"
#include "../EventBus/EventBus.h"
#include <sol/sol.hpp>

#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL.h>
#include <memory>

class WorldEditorStartEvent : public Event {

    public:
        sol::state& lua;
        std::unique_ptr<Registry>& registry;
        std::unique_ptr<AssetStore>& assetStore;
        SDL_Renderer* renderer;
        std::unique_ptr<EventBus>& eventBus;

        WorldEditorStartEvent(      
            sol::state& lua, 
            std::unique_ptr<Registry>& registry, 
            std::unique_ptr<AssetStore>& assetStore, 
            SDL_Renderer* renderer,
            std::unique_ptr<EventBus>& eventBus
        ) : lua(lua), registry(registry), assetStore(assetStore), renderer(renderer), eventBus(eventBus)  {}
};

#endif