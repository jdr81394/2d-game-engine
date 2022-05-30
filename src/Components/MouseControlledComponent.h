#ifndef MOUSECONTROLLEDCOMPONENT_H
#define MOUSECONTROLLEDCOMPONENT_H

#include <string>

struct MouseControlledComponent {

    bool isClickable;
    bool isDraggable;
    std::string link;
    

    MouseControlledComponent(
        bool isClickable = true, 
        bool isDraggable  = false, 
        std::string link = ""
        ) {}
};

#endif