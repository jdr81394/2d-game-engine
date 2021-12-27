#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include <glm/glm.hpp>

struct BoxColliderComponent {

    int width;
    int height;
    glm::vec2 offset;

    // offset is a glm::vec2 because you need width and height.
    // The glm library allows you to only pass one value to the glm::vec2 constructor to mean 0 for both values
    BoxColliderComponent(int width = 0, int height = 0, glm::vec2 offset = glm::vec2(0) ) {
        this->width = width;
        this->height = height;
        this->offset = offset;
    }
};


#endif