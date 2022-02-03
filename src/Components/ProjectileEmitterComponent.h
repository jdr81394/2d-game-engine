#ifndef PROJECTILEEMITTERCOMPONENT_H
#define PROJECTILEEMITTERCOMPONENT_H
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

struct ProjectileEmitterComponent {
    glm::vec2 projectileVelocity;
    int repeatFrequency;        // How frequently is it repeated.. in milliseconds
    int projectileDuration;     // How long should the projectile live for.
    int hitPercentageDamage;    // How much damage should this do?
    bool isFriendly;            // If the player shoulds a projectile, we should know that it cant hurt ourselves with our own projectile
    int lastEmissionTime;       // How many milliseconds ago was the last projectile emitted?

    ProjectileEmitterComponent(
        glm::vec2 projectileVelocity = glm::vec2(0),
        int repeatFrequency = 0,
        int projectileDuration = 10000,
        int hitPercentageDamage = 10,
        bool isFriendly = false
    ) {
        this->projectileVelocity = projectileVelocity;
        this->repeatFrequency = repeatFrequency;
        this->projectileDuration = projectileDuration;
        this->hitPercentageDamage = hitPercentageDamage;
        this->isFriendly = isFriendly;
        this->lastEmissionTime = SDL_GetTicks();
    };
};

#endif