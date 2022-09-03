#ifndef RESIZEWINDOWEVENT_H
#define RESIZEWINDOWEVENT_H

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

class ResizeWindowEvent : public Event {


    public:
        int id;
        ResizeWindowEvent(int a) {
            id = a;
        };

    private:

};


#endif