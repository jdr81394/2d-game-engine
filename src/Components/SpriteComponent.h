#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <glm/glm.hpp>
#include <string>
#include <SDL2/SDL.h>

struct SpriteComponent {
    std::string assetId;
    int width;
    int height;
    int zIndex;
    SDL_RendererFlip flip;
    bool isFixed;
    SDL_Rect srcRect;
    bool isAttachedToMouse;

    SpriteComponent(
        std::string assetId = "", 
        int width = 10, 
        int height = 10, 
        int zIndex = 0,
        bool isFixed = false,
        int srcRectX = 0, 
        int srcRectY = 0,
        bool isAttachedToMouse = false
        ) {
        this->assetId = assetId;
        this->width = width;
        this->height = height;
        this->zIndex = zIndex;
        this->flip = SDL_FLIP_NONE;
        this->isFixed = isFixed;
        this->srcRect = { srcRectX, srcRectY, width, height };
        this->isAttachedToMouse = isAttachedToMouse;
    }
};

#endif
