#ifndef RENDERHEALTHSYSTEM_H
#define RENDERHEALTHSYSTEM_H
#include "../ECS/ECS.h"
#include "../Components/HealthComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include <SDL.h>

class RenderHealthSystem : public System {

    public:
        RenderHealthSystem() {
            RequireComponent<HealthComponent>();
            RequireComponent<SpriteComponent>();
            RequireComponent<TransformComponent>();
        }

        void Update(SDL_Renderer* renderer, const SDL_Rect& camera) {
            
            for(auto entity : GetSystemEntities()) {
                auto healthComponent = entity.GetComponent<HealthComponent>();
                auto spriteComponent = entity.GetComponent<SpriteComponent>();
                auto transformComponent = entity.GetComponent<TransformComponent>();

                SDL_Rect healthBarOutline = {
                    static_cast<int>(transformComponent.position.x - camera.x),
                    static_cast<int>(transformComponent.position.y - 10 - camera.y),
                    static_cast<int>(spriteComponent.width),
                    6
                };
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &healthBarOutline);


                // Determine health bar color
                Uint8 r = 0;
                Uint8 g = 0;
                Uint8 b = 0;

    
                if(healthComponent.healthPercentage >= 75) {
                    g = 128; // Green
                } 
                else if(healthComponent.healthPercentage < 75 && healthComponent.healthPercentage >= 40) {
                    r = 255; // Orange
                    g = 165;
                }
                else {
                    r = 255; // Red
                }

                SDL_SetRenderDrawColor(renderer, r,g,b,255);

                // Draw Health Bar
                SDL_FRect healthBar = {
                    static_cast<float>(transformComponent.position.x - camera.x),
                    static_cast<float>(transformComponent.position.y - 9.5 - camera.y),
                    static_cast<float>(spriteComponent.width * healthComponent.healthPercentage / 100),
                    4.25
                };

                SDL_RenderFillRectF(renderer, &healthBar);


        

            }
        }
};

// float Percent = 1.0f * (healthComponent.healthPercentage / 100);
// SDL_Color old;
// // SDL_GetRenderDrawColor(renderer, 255, 255, 255, 255);
// SDL_Rect bgrect = { 
//         static_cast<int>(transformComponent.position.x - camera.x),
//         static_cast<int>(transformComponent.position.y - 10 - camera.y),
//         static_cast<int>(spriteComponent.width),
//         5
//  };
// SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
// SDL_RenderFillRect(renderer, &bgrect);
// SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
// int pw = (int)((float)bgrect.w * Percent);
// int px = bgrect.x + (bgrect.w - pw);
// SDL_Rect fgrect = { px, bgrect.y, pw, bgrect.h };
// SDL_RenderFillRect(renderer, &fgrect);
// SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


#endif