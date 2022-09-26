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

// enum TypeOfPress = { up = "Up" , down = "down", null = "null"}

// NOTE type of press up = 1, down = 0

// JAKE - MAKE ANOTHER CLASS FOR MOUSEPRESSEDEVENT WITH THE WORLDEDITOR RUNNING, this will avoid problem of not being able to initialize 
class MousePressedEvent : public Event {
        
    public:
        sol::state& lua;
        std::unique_ptr<Registry>& registry;
        std::unique_ptr<AssetStore>& assetStore;
        SDL_Renderer* renderer;
        std::unique_ptr<EventBus>& eventBus;
        bool isWorldEditor;
        int typeOfPress;
        int windowID;

        MousePressedEvent(      
            sol::state& lua, 
            std::unique_ptr<Registry>& registry, 
            std::unique_ptr<AssetStore>& assetStore, 
            SDL_Renderer* renderer,
            std::unique_ptr<EventBus>& eventBus,
            bool isWorldEditor,
            int typeOfPress,
            int windowID = 0
        ) : lua(lua), registry(registry), assetStore(assetStore), renderer(renderer), eventBus(eventBus), isWorldEditor(isWorldEditor),typeOfPress(typeOfPress),windowID(windowID)  {
            // this->isWorldEditor = isWorldEditor;
            // this->typeOfPress = typeOfPress;
            // this->windowID = windowID;

        }



};


#endif