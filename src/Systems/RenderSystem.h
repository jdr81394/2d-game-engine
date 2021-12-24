#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL.h>
class RenderSystem: public System {
    public:
        RenderSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
        }

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore ) {
            // Loop all entities that the system is interested in
            for (auto entity: GetSystemEntities()) {
                // Update entity position based on its velocity
                TransformComponent transform = entity.GetComponent<TransformComponent>();
                SpriteComponent sprite = entity.GetComponent<SpriteComponent>();

                // Set the source rectangle to our original sprite texture
                SDL_Rect srcRect = sprite.srcRect;

                // Set the destination rectangle with the x,y position to be rendered
                SDL_Rect dstRect = {
                    static_cast<int>(transform.position.x),
                    static_cast<int>(transform.position.y),
                    static_cast<int>(sprite.width * transform.scale.x),
                    static_cast<int>(sprite.height * transform.scale.y)
                };

                // The SDL render Copy Ex takes a rotation value as well
                SDL_RenderCopyEx(
                    renderer, 
                    assetStore->GetTexture(sprite.assetId),
                    &srcRect,
                    &dstRect,
                    transform.rotation, // See documentation for this and the following 3 properties - the point where it rotates around
                    NULL,               // defines center of rotation   
                    SDL_FLIP_NONE   // A SDL_RendererFlip 
                );

                // TODO: Draw the PNG texture 
            }
        }
};

#endif
