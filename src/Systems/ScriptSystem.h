#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H
#include "../ECS/ECS.h"
#include "../Components/ScriptComponent.h"

class ScriptSystem : public System {
    public:
        ScriptSystem(){
            RequireComponent<ScriptComponent>();
        };

        void Update() {
            for(auto entity : GetSystemEntities()) {
                const auto script = entity.GetComponent<ScriptComponent>();
                script.func();      // here is where we invoke a sol::function
            }
        }
};

#endif