#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H
#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/HealthComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class DamageSystem : public System {

    public:
        DamageSystem() {
            RequireComponent<BoxColliderComponent>();
        };

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus){
            // this = who is owner
            // &DamageSystem::onCollision refers to the actual function.
            // Not sure why the syntax is like this.
            eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
        };

        void onCollision(CollisionEvent& event) {
            Logger::Log(
                "The Damage system received an event collision between entities: "  + 
                std::to_string(event.a.GetId()) + " and " + 
                std::to_string(event.b.GetId())
            );

            Entity a = event.a;
            Entity b = event.b;

            if (a.BelongsToGroup("projectiles") && b.HasTag("player")) {
                OnProjectileHitsPlayer(a,b);        // A is player, b is projectile
            }

            if(b.BelongsToGroup("projectiles") && a.HasTag("player")) {
                OnProjectileHitsPlayer(b,a);        // B is projectile, a is player

            }

            if(a.BelongsToGroup("projectiles") && b.BelongsToGroup("enemies")) {

            }

            if (b.BelongsToGroup("projectiles") && a.BelongsToGroup("enemies")) {

            }


        };

        void OnProjectileHitsPlayer(Entity projectile, Entity player) {
            auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

            if(!projectileComponent.isFriendly) {
                // TODO: Reduce the health of the player by the projectileHitPercentDamage
                auto& health = player.GetComponent<HealthComponent>();

                // Subtract the health of the player
                health.healthPercentage -= projectileComponent.hitPercentDamage;

                // Kills the player when health reaches zero
                if(health.healthPercentage <= 0 ) {
                    player.Kill();
                }
                // Kills the projectile after it hits player.
                projectile.Kill();
            }

        }

        void Update() {

        };

};

#endif