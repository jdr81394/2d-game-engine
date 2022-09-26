#ifndef MOUSEPRESSEDWHILEMAPEDITOREVENT_H
#define MOUSEPRESSEDWHILEMAPEDITOREVENT_H

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"
#include "../EventBus/EventBus.h"
#include "./MousePressedEvent.h"
#include <sol/sol.hpp>

#include "../ECS/ECS.h"

#include "../AssetStore/AssetStore.h"
#include "../WorldEditor/WorldEditor.h"
#include <SDL2/SDL.h>
#include <memory>


class MousePressedWhileMapEditorEvent : public MousePressedEvent {

    public:  

        std::unique_ptr<Registry>& worldEditorRegistry ;

        MousePressedWhileMapEditorEvent(
            sol::state& lua, 
            std::unique_ptr<Registry>& registry, 
            std::unique_ptr<AssetStore>& assetStore, 
            SDL_Renderer* renderer,
            std::unique_ptr<EventBus>& eventBus,
            bool isWorldEditor,
            int typeOfPress,
            std::unique_ptr<Registry>& worldEditorRegistry ,
            int windowID = 0
            ): 
            MousePressedEvent(
                lua, 
                registry, 
                assetStore, 
                renderer,
                eventBus,
                isWorldEditor,
                typeOfPress,
                windowID
            ),  
            worldEditorRegistry(worldEditorRegistry) {
                // this->isWorldEditor = isWorldEditor;
                // this->typeOfPress = typeOfPress;
                // this->windowID = windowID;
        }

};



#endif