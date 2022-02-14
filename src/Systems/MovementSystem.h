#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
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
                auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

                transform.position.x += rigidbody.velocity.x * deltaTime; 
                transform.position.y += rigidbody.velocity.y * deltaTime; 


                if(entity.HasTag("player")) {
                    int paddingLeft = 10;
                    int paddingTop = 10;
                    int paddingRight = 50;
                    int paddingBottom = 50;

                    transform.position.x = transform.position.x < paddingLeft ? paddingLeft : transform.position.x;
                    transform.position.x = transform.position.x > Game::mapWidth - paddingRight ? Game::mapWidth - paddingRight : transform.position.x;
                    transform.position.y = transform.position.y > Game::mapHeight -  paddingBottom ? Game::mapHeight -  paddingBottom : transform.position.y;
                    transform.position.y = transform.position.y < paddingTop ? paddingTop : transform.position.y;

                }


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
