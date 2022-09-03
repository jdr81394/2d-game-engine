#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <string>

class RenderSystem: public System {
    public:
        RenderSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
        }

        void Update(
            SDL_Renderer* renderer, 
            std::unique_ptr<AssetStore>& assetStore, 
            SDL_Rect& camera
            ) {
            // Sort all the entities of our system by the z-index
            struct RenderableEntity {
                TransformComponent transformComponent;
                SpriteComponent spriteComponent;
                std::string group;
            };

            std::vector<RenderableEntity> renderableEntities;
            for (auto entity: GetSystemEntities()) {
                RenderableEntity renderableEntity;
                renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
                renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
        
                renderableEntity.group = entity.BelongsToGroup("World Editor") ? "World Editor" : "Main Screen";
                // Bypass rendering entities if they are outside the camera view

                 bool isEntityOutsideCameraView = (
                    renderableEntity.transformComponent.position.x + (renderableEntity.transformComponent.scale.x * renderableEntity.spriteComponent.width) < camera.x ||
                    renderableEntity.transformComponent.position.x > camera.x + camera.w ||
                    renderableEntity.transformComponent.position.y + (renderableEntity.transformComponent.scale.y * renderableEntity.spriteComponent.height) < camera.y ||
                    renderableEntity.transformComponent.position.y > camera.y + camera.h
                );

                if(isEntityOutsideCameraView && !renderableEntity.spriteComponent.isFixed) {
                    
                    continue;
                };

                // Push onto the back of the vector
                renderableEntities.emplace_back(renderableEntity);
            }
            
            // Sort the vector by the z-index value;
            // the std::sort takes a pointer to the first, last value in the vector, and lambda function to compare
            std::sort(
                renderableEntities.begin(), 
                renderableEntities.end(), 
                [](const RenderableEntity& a, const RenderableEntity& b) {
                    return a.spriteComponent.zIndex < b.spriteComponent.zIndex;         // This is the comparison statement. if a is less than b it is equal to true
                }
            );              

            
            // Loop all entities that the system is interested in
            for (auto entity: renderableEntities) {
                // Update entity position based on its velocity
                TransformComponent transform = entity.transformComponent;
                SpriteComponent sprite = entity.spriteComponent;

                // Set the source rectangle to our original sprite texture
                SDL_Rect srcRect = sprite.srcRect;

                // Set the destination rectangle with the x,y position to be rendered
                // Must take into account camera position now, because
                // depending on camera view, 
                // we have to chagne every entities position 
                // to subtract where my camera is
                // so if the camera moves to the right, everything must move to the left
                SDL_Rect dstRect = {
                    static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x) ),
                    static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y) ),
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
                    sprite.flip        // A SDL_RendererFlip 
                );
            



                // TODO: Draw the PNG texture 
            }
        }
};

#endif
