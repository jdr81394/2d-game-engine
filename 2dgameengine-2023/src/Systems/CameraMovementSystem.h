#ifndef CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/PlayerControlledComponent.h"
#include "../Components/TransformComponent.h"
#include <SDL.h>

class CameraMovementSystem: public System {
    public:
        CameraMovementSystem() {
            RequireComponent<PlayerControlledComponent>();
            RequireComponent<TransformComponent>();
        }

        void Update(SDL_Rect& camera) {
            for (auto entity: GetSystemEntities()) {
                const TransformComponent& transform = entity.GetComponent<TransformComponent>();

                if (transform.position.x + (camera.w / 2) < Game::mapWidth) {
                    camera.x = transform.position.x - (Game::windowWidth / 2);
                }

                if (transform.position.y + (camera.h / 2) < Game::mapHeight) {
                    camera.y = transform.position.y - (Game::windowHeight / 2);
                }

                // Keep camera rectangle view inside the screen limits
                camera.x = (camera.x + camera.w > Game::mapWidth) ? Game::mapWidth - camera.w : camera.x; // This is good for the right bounds
                camera.x = camera.x < 0 ? 0 : camera.x; // Checks the left bounds

                //camera.x = camera.x > 0 ? camera 
                camera.y = (camera.y + camera.h > Game::mapHeight) ? Game::mapHeight - camera.h : camera.y;
                camera.y = camera.y < 0 ? 0 : camera.y;

            }
        }
};

#endif
