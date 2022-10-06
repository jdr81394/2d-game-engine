#ifndef MOUSESYSTEM_H
#define MOUSESYSTEM_H
#include "../Components/MouseControlledComponent.h"
#include "../Events/MousePressedEvent.h"
#include <string>

class MouseSystem : public System {

    public:
        MouseSystem() {
            RequireComponent<MouseControlledComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->SubscribeToEvent<MousePressedEvent>(this, &MouseSystem::OnKeyPress);
        };

        void OnKeyPress(MousePressedEvent& event) {

            for(auto entity: GetSystemEntities()) {
                // const MouseControlledComponent mouseControlledComponent = entity.GetComponent<MouseControlledComponent>();

                // if(mouseControlledComponent.link.size() > 0) {
                //     const std::string link = mouseControlledComponent.link;
                //     Game::LoadLevel(link);
                // }
            }
        }

};


#endif