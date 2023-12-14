#ifndef RENDERHEALTHTEXTSYSTEM_H
#define RENDERHEALTHTEXTSYSTEM_H
#include "../ECS/ECS.h"
#include "../Components/HealthComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include <SDL.h>

class RenderHealthTextSystem : public System {
    public:
        RenderHealthTextSystem(){
            RequireComponent<HealthComponent>();
            RequireComponent<TextLabelComponent>();
            RequireComponent<SpriteComponent>();
            RequireComponent<TransformComponent>();
        }

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const SDL_Rect& camera) {
            for(Entity entity : GetSystemEntities()) {
                HealthComponent healthComponent = entity.GetComponent<HealthComponent>();
                TextLabelComponent textLabelComponent = entity.GetComponent<TextLabelComponent>();
                SpriteComponent spriteComponent = entity.GetComponent<SpriteComponent>();
                TransformComponent transformComponent = entity.GetComponent<TransformComponent>();

                // Put the healthPercentage as the content in the textLabelComponent
                textLabelComponent.text = std::to_string(healthComponent.healthPercentage) + "%";

                // Determine color of the health percentage text
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

                SDL_Color color = {r,g,b};

                SDL_Surface* surface = TTF_RenderText_Blended(
                    assetStore->GetFont(textLabelComponent.assetId),
                    textLabelComponent.text.c_str(),
                    color
                );

                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);

                int labelWidth;
                int labelHeight;

                SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);

                SDL_Rect dstRect = {
                    static_cast<int>(transformComponent.position.x + textLabelComponent.position.x - camera.x),
                    static_cast<int>(transformComponent.position.y + textLabelComponent.position.y - 25 - camera.y),
                    labelWidth,
                    labelHeight
                };

                SDL_RenderCopy(renderer, texture,NULL, &dstRect);

                SDL_DestroyTexture(texture);


               


            }
        }
};


#endif