#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <glm/glm.hpp>
#include <string>

struct SpriteComponent {
    int width;
    int height;
    std::string assetId;

    SpriteComponent(std::string assetId = "", int width = 10, int height = 10) {
        this->assetId = assetId;
        this->width = width;
        this->height = height;
    }
};

#endif
