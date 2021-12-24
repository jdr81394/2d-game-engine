#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <glm/glm.hpp>
#include <string>
#include <SDL2/SDL.h>

struct SpriteComponent {
    int width;
    int height;
    std::string assetId;
    SDL_Rect srcRect;

    SpriteComponent(std::string assetId = "", int width = 10, int height = 10, int srcRectX = 0, int srcRectY = 0) {
        this->assetId = assetId;
        this->width = width;
        this->height = height;
        this->srcRect = { srcRectX, srcRectY, width, height };
    }
};

#endif
