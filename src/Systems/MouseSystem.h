#ifndef MOUSESYSTEM_H
#define MOUSESYSTEM_H
#include "../Components/MouseControlledComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Events/MousePressedEvent.h"
#include "../Events/MousePressedWhileMapEditorEvent.h"
#include "../Events/MouseMotionToOtherWindowEvent.h"
#include "../ECS/ECS.h"
#include "../Systems/RenderSystem.h"
#include <string>
#include <memory>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "../Logger/Logger.h"
#include <iostream>

class MouseSystem : public System {

    public:
        MouseSystem() {
            RequireComponent<MouseControlledComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->SubscribeToEvent<MousePressedEvent>(this, &MouseSystem::OnKeyPress);
            eventBus->SubscribeToEvent<MousePressedWhileMapEditorEvent>(this, &MouseSystem::OnMapEditorKeyPress);
            eventBus->SubscribeToEvent<MouseMotionToOtherWindowEvent>(this, &MouseSystem::MotionToOtherWindow);

        };


        void OnKeyPress(MousePressedEvent& event) {

        }

        void MotionToOtherWindow(MouseMotionToOtherWindowEvent& event) {

            // Get anything thats being dragged. Anything that's being dragged will be in the render system, must switch registries.
            // Get the registry of the initial window
            int previousWindowId = event.previousWindowId;

            // If id == 1, then its the main screen, if id == 2 it equals the registry screen
            std::unique_ptr<Registry> & previousRegistry = previousWindowId == 1 ? event.registry : event.worldEditorRegistry;
            std::unique_ptr<Registry> & newRegistry = previousWindowId == 1 ? event.worldEditorRegistry : event.registry;

            // Get all the entities that are being dragged, normally should only be one. Do this by looping through the system of render of the registry
            
             
            
            for (Entity entity : previousRegistry->GetSystem<RenderSystem>().GetSystemEntities()) {
                SpriteComponent spriteComponent = entity.GetComponent<SpriteComponent>();
                Logger::Log("outside of here");

                if(spriteComponent.isAttachedToMouse) {
                    
                    Logger::Log("in here");
                    TransformComponent transformComponent = entity.GetComponent<TransformComponent>();

                    // Should only be two components while dragging tiles. Let's create a new entity in the new windows registry
                    Entity tile = newRegistry->CreateEntity();
                    
                    tile.AddComponent<TransformComponent>(
                        transformComponent.position,
                        transformComponent.scale,
                        transformComponent.rotation
                    );

                    tile.AddComponent<SpriteComponent>(
                        spriteComponent.assetId,
                        spriteComponent.width,
                        spriteComponent.height,
                        spriteComponent.zIndex,
                        spriteComponent.srcRect.x,
                        spriteComponent.srcRect.y,
                        spriteComponent.isAttachedToMouse
                    );

                    // Now kill this old entity on the old registry
                    entity.Kill();

                }
            }


            

        }

        void OnMapEditorKeyPress(MousePressedWhileMapEditorEvent& event) {
            int typeOfPress = event.typeOfPress;    // up = 1, down = 0
            int windowID = event.windowID;
            
            Logger::Log("window Id: " + std::to_string(windowID));
            // if down
            if(typeOfPress == 0) { 
                Logger::Log("press down");

                if(windowID == 1) {


                }

                if(windowID == 2) {

                    // use map editor registry to get the entity that is at that location - JAKE DO THIS NEXT
                    std::unique_ptr<Registry>&  registry = event.worldEditorRegistry;

                    RenderSystem & renderSystem = registry->GetSystem<RenderSystem>();

                    for( Entity entity : renderSystem.GetSystemEntities() ) {

                        TransformComponent transformComponent = entity.GetComponent<TransformComponent>();
                        SpriteComponent & spriteComponent = entity.GetComponent<SpriteComponent>();

                        int x, y = 0;

                        SDL_GetMouseState(&x, &y);

                        glm::vec2 mouseCoordinates = glm::vec2(static_cast<int>(x),static_cast<int>(y));

                        double initialX = transformComponent.position.x + spriteComponent.width;
                        double initialY = transformComponent.position.y + spriteComponent.height;
                        double width    = spriteComponent.width;
                        double height   = spriteComponent.height;

                        bool isClickInEntity = CheckAABBCollision(
                                initialX,
                                initialY,
                                width,
                                height,
                                mouseCoordinates.x,
                                mouseCoordinates.y,
                                5.0,
                                5.0
                        );

                        if(isClickInEntity) {

                            Logger::Log("In entity, here is id: " + std::to_string(entity.GetId()));
                            // JAKE - Must fix the fact that some of the sprites dont line up on where they are in the screen

                            spriteComponent.isAttachedToMouse = true;


                        }

                    }
                }
            } 
            
            // if up
            if(typeOfPress == 1) {
                Logger::Log("press up");

                // main
                if(windowID == 1) {

                }    
                // map editor
                if (windowID == 2) {

                    Logger::Log("in window id");

                }  
            }

        }


        bool CheckAABBCollision(
            double aX, 
            double aY, 
            double aW, 
            double aH,
            double bX,
            double bY,
            double bW,
            double bH    
        ) {
        // This is the AABB collision algorithm. 
        // You can find it here https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
        return (
            aX < bX + bW &&
            aX + aW > bX &&
            aY < bY + bH && 
            aY + aH > bY 
        );
    }; 





};


#endif