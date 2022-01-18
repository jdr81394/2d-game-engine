#ifndef EVENTBUS_H
#define EVENTBUS_H
#include "../Logger/Logger.h"

class EventBus {
    public:
        EventBus(){
            Logger::Log("Event bus constructor has been called");
        }
        ~EventBus(){
            Logger::Log("Event bus destructor has been called");
        }

        /* 
            Subscribe to event type <T>
            In our implementation, a listener subscribes to an event
            Example: eventBus->SubscribeToEvent<CollisionEvent>(&Game::onCollision)
        */
        // Some implementations call this Listen To Event
        template <typename T>
        void SubscribeToEvent<>() {
            //TODO:
        }
        /*
            Emit an event of type <T>
            In our implementation, as soon as something emits an event we go ahead and execute all those listener callbacks
            Example: eventBus->EmitEvent<CollisionEvent>(player,enemy);
        */
        // Some implementations call this "dispatch Event"
        template <typename T>
        void EmitEvent<>() {
            // TODO:
        }
}


#endif