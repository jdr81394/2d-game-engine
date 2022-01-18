#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "../ECS/ECS.h"
#include "./Event.h"

class CollisionEvent : public Event {


    public:
        Entity a;
        Entity b;
        // This line: " : a(a), b(b) {} "
        // is alternative syntax for 
        // this->a=a;
        // this->b=b;
        // and the {} brackets are the implementation.
        CollisionEvent(Entity a, Entity b): a(a),b(b) {};

    private:

};


#endif