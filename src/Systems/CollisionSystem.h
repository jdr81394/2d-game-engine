#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../EventBus/EventBus.h"

class CollisionSystem : public System {

    public:
        CollisionSystem() {
            RequireComponent<BoxColliderComponent>();
            RequireComponent<TransformComponent>();
        }

    void Update(std::unique_ptr<EventBus>& eventBus) {
        // TODO: Check all entities that have a box collider to see if they are colliding with each other.
        // Tip: AABB collision check = Axis-Aligned Bounding Boxes

        auto entities = GetSystemEntities();

        for (auto i = entities.begin(); i != entities.end(); i++) {

            Entity a = *i;      // This i* deferences the pointer and gives me the actual object

            auto aTransform = a.GetComponent<TransformComponent>();
            auto aCollider = a.GetComponent<BoxColliderComponent>();

            for (auto j = i; j != entities.end(); j++) {
                Entity b = *j;
                
                if(a == b) continue;

                auto bTransform = b.GetComponent<TransformComponent>();
                auto bCollider = b.GetComponent<BoxColliderComponent>();

                // Perform with AABB collision check between entities a and b
                // Put the position and the offset together for the position.
                bool collisionHappened = CheckAABBCollision(
                    aTransform.position.x + aCollider.offset.x,
                    aTransform.position.y + aCollider.offset.y,
                    aCollider.width,
                    aCollider.height,
                    bTransform.position.x + bCollider.offset.x,
                    bTransform.position.y + bCollider.offset.y,
                    bCollider.width,
                    bCollider.height
                );

                if(collisionHappened) {
                    // Trigger event
                    Logger::Log(
                        "Entity " + 
                        std::to_string(a.GetId()) + 
                        " is colliding with " + 
                        std::to_string(b.GetId())
                    );
                    
                    // TODO: emit an event
                    eventBus->EmitEvent<CollisionEvent>(a, b);
                }
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