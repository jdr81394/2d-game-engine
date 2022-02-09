#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class MovementSystem: public System {
    public:
        MovementSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<RigidBodyComponent>();
        }

        void Update(double deltaTime) {
            // Loop all entities that the system is interested in
            for (auto entity: GetSystemEntities()) {
                // Update entity position based on its velocity
                auto& transform = entity.GetComponent<TransformComponent>();
                const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

                transform.position.x += rigidbody.velocity.x * deltaTime; 
                transform.position.y += rigidbody.velocity.y * deltaTime; 

                bool isEntityOutsideMap = (
                    transform.position.x < 0 || 
                    transform.position.x > Game::mapWidth ||
                    transform.position.y < 0 ||
                    transform.position.y > Game::mapHeight
                );

                // We want to kill everything but our player
                if(isEntityOutsideMap && !entity.HasTag("player")) {
                    entity.Kill();
                }
            }
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus){
            eventBus->SubscribeToEvent<CollisionEvent>(this, &MovementSystem::onCollision);
        };

        void onCollision(CollisionEvent& event) {
            Logger::Log(
                "The Movement system received an event collision between entities: "  + 
                std::to_string(event.a.GetId()) + " and " + 
                std::to_string(event.b.GetId())
            );

            Entity a = event.a;
            Entity b = event.b;

            if (a.BelongsToGroup("enemies") && b.BelongsToGroup("obstacles")) {
                OnEnemyHitsObstacle(a,b);        // A is an enemy, b is an obstacle
            }

            if(b.BelongsToGroup("enemies") && a.BelongsToGroup("obstacles")) {
                OnEnemyHitsObstacle(b,a);        // B is enemy, a is obstacle
            }


        };

        void OnEnemyHitsObstacle(Entity enemy, Entity obstacle) {
            if(enemy.HasComponent<RigidBodyComponent>() && enemy.HasComponent<SpriteComponent>()) {
                auto& rigidbody = enemy.GetComponent<RigidBodyComponent>();
                auto& sprite = enemy.GetComponent<SpriteComponent>();

                if(rigidbody.velocity.x != 0) {
                    rigidbody.velocity.x *= -1;
                    sprite.flip = (sprite.flip == SDL_FLIP_NONE) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
                }
                if(rigidbody.velocity.y !=0) {
                    rigidbody.velocity.y *= -1;
                    sprite.flip = (sprite.flip == SDL_FLIP_NONE) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;

                }
                
            }
        };
};

#endif
