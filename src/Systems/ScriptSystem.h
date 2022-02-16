#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Components/ScriptComponent.h"
#include "../Components/TransformComponent.h"
#include <sol/sol.hpp>

// int GetEntityPosition(Entity entity) {

// }

void SetEntityPosition(Entity entity, double x, double y) {
    if(entity.HasComponent<TransformComponent>()) {
        auto& transformComponent = entity.GetComponent<TransformComponent>();
        transformComponent.position.x = x;
        transformComponent.position.y = y;
    } else {
        Logger::Err("Trying to set the position of an entity that has no transform component");
    }
}



class ScriptSystem : public System {
    public:
        ScriptSystem(){
            RequireComponent<ScriptComponent>();
        };

        void CreateLuaBindings(sol::state& lua) {
            
            // We need to create the "entity" usertype so Lua knows what an entity is
            lua.new_usertype<Entity>(
                "entity",
                "get_id", &Entity::GetId,
                "destroy",&Entity::Kill,
                "has_tag",&Entity::HasTag,
                "belongs_to_group",&Entity::BelongsToGroup
            );

            //TODO Create all the bindings between lua and C++ 
            lua.set_function("set_position", SetEntityPosition);
            // lua.set_function("get_position", GetEntityPosition);
        }

        void Update(double deltaTime, int ellapsedTime) {
            for(auto entity : GetSystemEntities()) {
                const auto script = entity.GetComponent<ScriptComponent>();
                script.func(entity, deltaTime, ellapsedTime);      // here is where we invoke a sol::function
            }
        }
};

#endif