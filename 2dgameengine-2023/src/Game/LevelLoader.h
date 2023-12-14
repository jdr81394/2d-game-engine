#ifndef LEVELLOADER_H
#define LEVELLOADER_H
#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include <memory>
#include <SDL.h>
#include "../Logger/Logger.h"
#include <sol/sol.hpp>

class LevelLoader {

    public:
        LevelLoader();
        ~LevelLoader();

        void LoadLevel(sol::state& lua, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, int levelNumber);


};

#endif