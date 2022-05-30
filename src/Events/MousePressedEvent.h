#ifndef MOUSEPRESSEDEVENT_H
#define MOUSEPRESSEDEVENT_H

#include "../EventBus/Event.h"
#include "../ECS/ECS.h"
#include <SDL2/SDL.h>

class MousePressedEvent : public Event {
    public:
        SDL_MouseButtonEvent symbol;
        MousePressedEvent(SDL_MouseButtonEvent symbol): symbol(symbol) {};


};


#endif