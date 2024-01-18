#ifndef EVENTBUS_H
#define EVENTBUS_H
#include "../Logger/Logger.h"
#include "Event.h"
#include <map>
#include <typeindex>
#include <list>
#include <memory>


// This base class is created so we can use this class
// as a Type when we want to use the EventCallback as a generic 
// in variable declarations.
class IEventCallback {
    private: 
        // the "= 0" makes the call function a pure virtual class. C++ interprets it that way.
        virtual void Call(Event& e) = 0;

    public: 
        // default makes the compiler generate the defalt one..
        virtual ~IEventCallback() = default;
        void Execute(Event& e) {
            Call(e);
        };
};


// Basically a little wrapper around a function pointer
// The TEvent is going determine what kind of callback it is. 
// Such as a collision event callback.
// Here we have the callback function pointer 
// that needs to be invoked
template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback {
   private:
        // Inside each one of these event callback functions, were going to
        // keep track of the instance that owns this function owner.
        // And the callback function that we need to invoke.
        
        // We are passing TEvent as the eventtype of that callback function
        typedef void (TOwner::*CallbackFunction)(TEvent&);      // Gustavo even says this is weird
        TOwner* ownerInstance;
        CallbackFunction callbackFunction;

        virtual void Call(Event& e) override {
            // 
            std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
        };

    public:
        EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction) {
            this->ownerInstance = ownerInstance;
            this->callbackFunction = callbackFunction;
        }

        virtual ~EventCallback() override = default;

};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;


class EventBus {
    private:
        // We will create the handler list class.
        // The handleList is a collection of callback functions that need to be executed.
        std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;
    public:
        EventBus(){
            Logger::Log("Event bus constructor has been called");
        }
        ~EventBus(){
            Logger::Log("Event bus destructor has been called");
        }

        void Reset() {
            subscribers.clear();
        }

        /* 
            Subscribe to event type <T>
            In our implementation, a listener subscribes to an event
            Example: eventBus->SubscribeToEvent<CollisionEvent>(&Game::onCollision)
            // Its passing the object instance owner as well as the function
        */
        // Some implementations call this Listen To Event
        template <typename TEvent, typename TOwner>
        void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&) ) {
            if(!subscribers[typeid(TEvent)].get()) {
                subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
            }
            //TODO:
            auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);//;
            
            /*
            Since the subscriber is going to be a unique pointer, we need to use the std::move when we push to map.
            This is C++ move semantics, where we can use std::move to change ownership of the object from one unique ptr to another unique ptr
            */
            subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
        }
        /*
            Emit an event of type <T>
            In our implementation, as soon as something emits an event we go ahead and execute all those listener callbacks
            Example: eventBus->EmitEvent<CollisionEvent>(
            ,enemy);
        */
        // Some implementations call this "dispatch Event"
        // TArgs means multiple arguments
        template <typename TEvent, typename ...TArgs>
        void EmitEvent(TArgs&& ...args) {
            // TODO:
            auto handlers = subscribers[typeid(TEvent)].get();

            if(handlers) {
                for (auto it = handlers->begin(); it != handlers->end(); it++ ) {
                    auto handler = it->get();
                    TEvent event(std::forward<TArgs>(args)...);
                    handler->Execute(event);
                }
            }
        }
};


#endif