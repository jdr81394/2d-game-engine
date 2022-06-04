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
        WorldEditorStartEvent() {}


};

#endif