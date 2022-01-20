#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H
#include "../ECS/ECS.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../src/Events/KeyPressedEvent.h"
#include <SDL2/SDL.h>
#include "../Logger/Logger.h"
#include "../EventBus/EventBus.h"
#include <string>

class KeyboardControlSystem : public System {

    public:
        KeyboardControlSystem() {

        };

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPress);
        };

        void OnKeyPress(KeyPressedEvent& event) {
            std::string keyCode = std::to_string(event.symbol);
            std::string keySymbol(1, event.symbol);
            Logger::Log("Key pressed event emitted: [" + keyCode + "] " + keySymbol);
        }

};


#endif
