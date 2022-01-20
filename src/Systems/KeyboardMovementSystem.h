#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H
#include "../ECS/ECS.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../src/Events/KeyPressedEvent.h"
#include <SDL2/SDL.h>
#include "../Logger/Logger.h"
#include "../EventBus/EventBus.h"

class KeyboardMovementSystem : public System {

    public:
        KeyboardMovementSystem() {

        };

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardMovementSystem::OnKeyPress);
        };

        void OnKeyPress(KeyPressedEvent& event) {
            Logger::Log("HERE IT IS IN ONKEYPRESS");
        }

        void Update(std::unique_ptr<EventBus>& eventBus) {
            SDL_Event event;

            while(SDL_PollEvent(&event)){
                if(event.key.keysym.sym) {
                    Logger::Log("In event key");
                    eventBus->EmitEvent<KeyPressedEvent>(event.key.keysym.sym);
                    Logger::Log("here is the key: " + std::to_string(event.key.keysym.sym));
                }
            }
        }

};


#endif
