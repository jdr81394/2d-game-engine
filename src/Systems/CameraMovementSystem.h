#ifndef CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H
#include "../ECS/ECS.h"
#include <SDL2/SDL.h>
#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"


class CameraMovementSystem : public System {
    public:
    CameraMovementSystem() {
        RequireComponent<CameraFollowComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update(SDL_Rect& camera) {
        for (auto entity : GetSystemEntities()) {
            auto transform = entity.GetComponent<TransformComponent>();

            // change camera.x and camera.y based on the entity transformp osition
            camera.x = transform.position.x;
            camera.y = transform.position.y;
            Logger::Log("Info for transform: " + std::to_string(camera.x) + " y value: " + std::to_string(camera.y));
        }
    }
};

#endif