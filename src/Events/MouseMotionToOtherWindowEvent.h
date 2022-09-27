#ifndef MOUSEMOTIONTOOTHERWINDOWEVENT_H
#define MOUSEMOTIONTOOTHERWINDOWEVENT_H

#include "../EventBus/Event.h"
#include "../ECS/ECS.h"
#include <SDL2/SDL.h>

class MouseMotionToOtherWindowEvent  : public Event {
    public:
        int previousWindowId;
        int newWindowId;
        std::unique_ptr<Registry>& registry;
        std::unique_ptr<Registry>& worldEditorRegistry;
        MouseMotionToOtherWindowEvent(        
            int previousWindowId,
            int newWindowId,
            std::unique_ptr<Registry>& registry, 
            std::unique_ptr<Registry>& worldEditorRegistry
        ): previousWindowId(previousWindowId), newWindowId(newWindowId),registry(registry),worldEditorRegistry(worldEditorRegistry)  {

        };
};



#endif