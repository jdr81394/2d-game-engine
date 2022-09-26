#ifndef ECS_H
#define ECS_H

#include "../Logger/Logger.h"
#include <vector>
#include <bitset>
#include <set>
#include <deque>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <string>
#include <iostream>
const unsigned int MAX_COMPONENTS = 32;

////////////////////////////////////////////////////////////////////////////////
// Signature
////////////////////////////////////////////////////////////////////////////////
// We use a bitset (1s and 0s) to keep track of which components an entity has,
// and also helps keep track of which entities a system is interested in.
////////////////////////////////////////////////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
    protected:
        static int nextId;
};

// Used to assign a unique id to a component type
template <typename T>
class Component: public IComponent {
    public:
        // Returns the unique id of Component<T>
        static int GetId() {
            static auto id = nextId++;
            return id;
        }
};

class Entity {
    private:
        int id;

    public:
        Entity(int id): id(id) {};
        Entity(const Entity& entity) = default;
        int GetId() const;
       
        Entity& operator =(const Entity& other) = default;
        bool operator ==(const Entity& other) const { return id == other.id; }
        bool operator !=(const Entity& other) const { return id != other.id; }
        bool operator >(const Entity& other) const { return id > other.id; }
        bool operator <(const Entity& other) const { return id < other.id; }

        template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
        template <typename TComponent> void RemoveComponent();
        template <typename TComponent> bool HasComponent() const;
        template <typename TComponent> TComponent& GetComponent() const;

        void Kill();

        // Manage entity tags and groups
        void Tag(const std::string& tag);
        bool HasTag(const std::string& tag) const;
        void Group(const std::string& group);
        bool BelongsToGroup(const std::string& group) const;

        // Hold a pointer to the entity's owner registry
        class Registry* registry;
};

////////////////////////////////////////////////////////////////////////////////
// System
////////////////////////////////////////////////////////////////////////////////
// The system processes entities that contain a specific signature
////////////////////////////////////////////////////////////////////////////////
class System {
    private:
        Signature componentSignature;
        std::vector<Entity> entities;

    public:
        System() = default;
        ~System() = default;
        
        void AddEntityToSystem(Entity entity);
        void RemoveEntityFromSystem(Entity entity);
        std::vector<Entity> GetSystemEntities() const;
        const Signature& GetComponentSignature() const;

        // Defines the component type that entities must have to be considered by the system
        template <typename TComponent> void RequireComponent();
};

////////////////////////////////////////////////////////////////////////////////
// Pool
////////////////////////////////////////////////////////////////////////////////
// A pool is just a vector (contiguous data) of objects of type T
////////////////////////////////////////////////////////////////////////////////
class IPool {
    public:
        virtual ~IPool() = default;


        // This allows us to call RemoveEntityFromPool from the componentPools since this
        // data structure is actually composed of IPool
        virtual void RemoveEntityFromPool(int entityId) = 0;

};

template <typename T>
class Pool: public IPool {
    private:
        std::vector<T> data;
        int size;

        std::unordered_map<int,int> entityIdToIndex;
        std::unordered_map<int,int> indexToEntityId;

    public:
        Pool(int capacity = 100) {
            size = 0;
            data.resize(capacity);
        }

        virtual ~Pool() = default;

        bool IsEmpty() const {
            return size == 0;
        }

        int GetSize() const {
            return size;
        }

        void Resize(int n) {
            data.resize(n);
        }

        void Clear() {
            data.clear();
            size = 0;
        }

        void Add(T object) {
            data.push_back(object);
        }

        // Main method to add an entity to the data structures
        // When adding a new object, keep track of the entity id and its vector index
        void Set(int entityId, T object) {
            // If element already already exists, it will be found and not reach end. 
            // Therefore, just replace the component object
            if(entityIdToIndex.find(entityId) != entityIdToIndex.end()) {

                int index = entityIdToIndex[entityId];
                data[index] = object;
            } 
            // If it reaches the end, it doesnt exist, therefore it must be created.
            else {
                // When adding a new object, we keep track of the entity ids and their vector index
                int index = size;
                entityIdToIndex.emplace(entityId, index);
                indexToEntityId.emplace(index, entityId);
                if(index >= data.capacity()) {
                    // If necessary, we resize by always doubling the current capacity
                    data.resize(size * 2);
                }
                data[index] = object;
                size++;
            }
        }

        void Remove(int entityId) {
            /*
                    entityIdToIndex				indexToEntityId             data
                    [entityId][index]			[index][entityId]
                    [0]	=> 	[0]				    [0]	=>	[0]                 [0]
Remove this one ->  [5]	=>	[1]				    [1]	=>	[5]                 [5]
                    [12]	=>	[2]				[2]	=>	[12]                [12]
                    [15]	=>	[3]				[3]	=>	[15]                [15]
                    [22]    =>   [4]            [4]  => [22]                [22]

            entityId = 5
            */
            // Copy the last element to the deleted position to keep the array packed
            //       1               = entityIdToIndex[5]
            int indexOfRemoved = entityIdToIndex[entityId];     // The removed values index
            //      4       = 5 - 1
            int indexOfLast = size - 1;                         // The final values index
            // data[1]  (5) now is 22  = data[4] (22)
            data[indexOfRemoved] = data[indexOfLast];           // Place the value at the final index, at the removed values index


            /*

                    entityIdToIndex				indexToEntityId             data
                    [entityId][index]			[index][entityId]
                    [0]	=> 	[0]				    [0]	=>	[0]                 [0]     
Remove this one ->  [5]	=>	[1]				    [1]	=>	[5]                 [22] <---     < This was the change
                    [12]	=>	[2]				[2]	=>	[12]                [12]    |
                    [15]	=>	[3]				[3]	=>	[15]                [15]    |
                    [22]    =>   [4]            [4]  => [22]                [22] ----
                
                current values:
                entityId = 5
                indexOfRemoved = 1;
                indexOfLast = 4;
            */

            // Update the index-entity maps to point to the correct elements
                //        22          =   indexToEntityId[4] 
            int entityIdOfLastElement = indexToEntityId[indexOfLast] ; // Get the element id that was at the last element
            //  entityIdToIndex[22]                = 1
            entityIdToIndex[entityIdOfLastElement] = indexOfRemoved;
            
            /*

                    entityIdToIndex				indexToEntityId             data
                    [entityId][index]			[index][entityId]
                    [0]	=> 	[0]				    [0]	=>	[0]                 [0]     
                    [5]	=>	[1]				    [1]	=>	[5]                 [22]
                    [12]	=>	[2]				[2]	=>	[12]                [12]    
    change made     [15]	=>	[3]				[3]	=>	[15]                [15]    
         ---------> [22]    =>  [1] <----       [4]  => [22]                [22] 
                
                current values:
                entityId = 5
                indexOfRemoved = 1;
                indexOfLast = 4;
            */


            // indexToEntityId[1]            =  22
            indexToEntityId[indexOfRemoved] = entityIdOfLastElement;

            /*

                    entityIdToIndex				indexToEntityId             data
                    [entityId][index]			[index][entityId]
                    [0]	=> 	[0]				    [0]	=>	[0]  the change      [0]     
                    [5]	=>	[1]				    [1]	=>	[22] <-------       [22]
                    [12]	=>	[2]				[2]	=>	[12]                [12]    
                    [15]	=>	[3]				[3]	=>	[15]                [15]    
                    [22]    =>  [1]             [4]  => [22]                [22]
                
                current values:
                entityId = 5
                indexOfRemoved = 1;
                indexOfLast = 4;
            */

            entityIdToIndex.erase(entityId);
            indexToEntityId.erase(indexOfLast);
            
            /*

                    entityIdToIndex				indexToEntityId             data
                    [entityId][index]			[index][entityId]
                    [0]	=> 	[0]				    [0]	=>	[0]                  [0]     
        erased ---> [5]	=>	[1]				    [1]	=>	[22]                [22]
                    [12]	=>	[2]				[2]	=>	[12]                [12]    
                    [15]	=>	[3]				[3]	=>	[15]                [15]    
                    [22]    =>  [1]             [4]  => [22] <--- erased    [22]
                
                current values:
                entityId = 5
                indexOfRemoved = 1;
                indexOfLast = 4;


                    entityIdToIndex				indexToEntityId             data
                    [entityId][index]			[index][entityId]
                    [0]	=> 	[0]				    [0]	=>	[0]                  [0]     
                            				    [1]	=>	[22]                [22]
                    [12]	=>	[2]				[2]	=>	[12]                [12]    
                    [15]	=>	[3]				[3]	=>	[15]                [15]    
                    [22]    =>  [1]                                         [22] ???


            */

            size--;
        }

        void RemoveEntityFromPool(int entityId) override {
            if(entityIdToIndex.find(entityId) != entityIdToIndex.end()) {
                Remove(entityId);
            }
        }

        T& Get(int entityId) {
            int index = entityIdToIndex[entityId];
            return static_cast<T&>(data[index]);
        }

        T& operator [](unsigned int index) {
            return data[index];
        }
};

////////////////////////////////////////////////////////////////////////////////
// Registry
////////////////////////////////////////////////////////////////////////////////
// The registry manages the creation and destruction of entities, add systems,
// and components.
////////////////////////////////////////////////////////////////////////////////
class Registry {
    private:
        int numEntities = 0;

        // Vector of component pools, each pool contains all the data for a certain compoenent type
        // [Vector index = component type id]
        // [Pool index = entity id]
        std::vector<std::shared_ptr<IPool>> componentPools;

        // Vector of component signatures per entity, saying which component is turned "on" for a given entity
        // [Vector index = entity id]
        std::vector<Signature> entityComponentSignatures;

        // Map of active systems
        // [Map key = system type id]
        std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

        // Set of entities that are flagged to be added or removed in the next registry Update()
        std::set<Entity> entitiesToBeAdded;
        std::set<Entity> entitiesToBeKilled;

        // Entity tags (one tag name per entity)
        std::unordered_map<std::string, Entity> entityPerTag;
                        // EntityId
        std::unordered_map<int, std::string> tagPerEntity;

        // Entity groups (a set of entities per group name)
        std::unordered_map<std::string, std::set<Entity>> entitiesPerGroup;
                        // EntityId
        std::unordered_map<int, std::string> groupPerEntity;

        int currentMouseOverWindowId;


        std::deque<int> freeIds;

    public:
        Registry() {
            // Logger::Log("Registry constructor called");
        }
        
        ~Registry() {
            // Logger::Log("Registry destructor called");
        }

        // The registry Update() finally processes the entities that are waiting to be added/killed to the systems
        void Update();
        
        // Entity management
        Entity CreateEntity();
        int GetNumEntities();

        // Remove Entity
        void KillEntity(Entity entity);

        // Component management
        template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
        template <typename TComponent> void RemoveComponent(Entity entity);
		template <typename TComponent> bool HasComponent(Entity entity) const;
        template <typename TComponent> TComponent& GetComponent(Entity entity) const;

        // System management
        template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
        template <typename TSystem> void RemoveSystem();
        template <typename TSystem> bool HasSystem() const;
        template <typename TSystem> TSystem& GetSystem() const;

        // Tag management
        void TagEntity(Entity entity, const std::string& tag);
        bool EntityHasTag(Entity entity, const std::string& tag) const;
        Entity GetEntityByTag(const std::string& tag) const;
        void RemoveEntityTag(Entity entity);

        // Group Management
        void GroupEntity(Entity entity, const std::string& group);
        bool EntityBelongsToGroup(Entity entity, const std::string& group) const;
        std::vector<Entity> GetEntitiesByGroup(const std::string& group) const;
        void RemoveEntityGroup(Entity entity);

        // Checks the component signature of an entity and add the entity to the systems
        // that are interested in it
        void AddEntityToSystems(Entity entity);
        void RemoveEntityFromSystems(Entity entity);

        // Level Management
        void DeloadEverything();
        void UpdateCurrentMouseOverWindowId(int id);
        int GetCurrentMouseOverWindowId();
};

template <typename TComponent>
void System::RequireComponent() {
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

template <typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs&& ...args) {
    std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template <typename TSystem>
void Registry::RemoveSystem() {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(system);
}

template <typename TSystem>
bool Registry::HasSystem() const {
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem>
TSystem& Registry::GetSystem() const {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(system->second));
}

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args) {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    if (componentId >= componentPools.size()) {
        componentPools.resize(componentId + 1, nullptr);
    }

    if (!componentPools[componentId]) {
        std::shared_ptr<Pool<TComponent>> newComponentPool(new Pool<TComponent>());
        componentPools[componentId] = newComponentPool;
    }
    // Let's get the newly created componentPool thats related to the entities that would have been created in the last if statement.
    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

    TComponent newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityId, newComponent);

    entityComponentSignatures[entityId].set(componentId);

    // Logger::Log("Component id = " + std::to_string(componentId) + " was added to entity id " + std::to_string(entityId));

    // std::cout << "COMPONENT ID = " << componentId << " ---> POOL SIZE: " << componentPool->GetSize() << std::endl;
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
    
    // Remove the component from the component list for that entity
    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    componentPool->Remove(entityId);

    // Set this component signature for that entity to false
	entityComponentSignatures[entityId].set(componentId, false);
    
    // Logger::Log("Component id = " + std::to_string(componentId) + " was removed from entity id " + std::to_string(entityId));
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	return entityComponentSignatures[entityId].test(componentId);
}

template <typename TComponent>
TComponent& Registry::GetComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
    auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    return componentPool->Get(entityId);
}

template <typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs&& ...args) {
    registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void Entity::RemoveComponent() {
    registry->RemoveComponent<TComponent>(*this);
}

template <typename TComponent>
bool Entity::HasComponent() const {
    return registry->HasComponent<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::GetComponent() const {
    return registry->GetComponent<TComponent>(*this);
}

#endif
