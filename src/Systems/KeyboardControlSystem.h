#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H
#include "../ECS/ECS.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../src/Events/KeyPressedEvent.h"
#include <SDL2/SDL.h>
#include "../Logger/Logger.h"
#include "../EventBus/EventBus.h"
#include <string>

class KeyboardControlSystem : public System {

    public:
        KeyboardControlSystem() {
            RequireComponent<SpriteComponent>();
            RequireComponent<RigidBodyComponent>();
            RequireComponent<KeyboardControlledComponent>();
        };

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPress);
        };

        void OnKeyPress(KeyPressedEvent& event) {
            // std::string keyCode = std::to_string(event.symbol);
            // std::string keySymbol(1, event.symbol);
            // Logger::Log("Key pressed event emitted: [" + keyCode + "] " + keySymbol);
            for (auto entity : GetSystemEntities()) {
                const auto keyboardcontrol = entity.GetComponent<KeyboardControlledComponent>();
                auto& rigidbody = entity.GetComponent<RigidBodyComponent>();
                auto& sprite = entity.GetComponent<SpriteComponent>();

                switch (event.symbol) {
                    case SDLK_UP:
                        /* */
                        rigidbody.velocity = keyboardcontrol.upVelocity;
                        sprite.srcRect.y = sprite.height * 0;       // Points to first row. This works because the sprite is already 32 pixels tall, so multiplying it by 0 gives the proper placement for where we want to be in the spritesheet
                        
                        break;
                    case SDLK_RIGHT:
                        rigidbody.velocity = keyboardcontrol.rightVelocity;
                        sprite.srcRect.y = sprite.height * 1;
                        break;
                    case SDLK_DOWN:
                        rigidbody.velocity = keyboardcontrol.downVelocity;
                        sprite.srcRect.y = sprite.height * 2;
                        break;
                    case SDLK_LEFT:
                        rigidbody.velocity = keyboardcontrol.leftVelocity;
                        sprite.srcRect.y = sprite.height * 3;
                        break;
                }
            }
        }

};


#endif
