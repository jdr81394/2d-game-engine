#ifndef LEVELLOADER_H
#define LEVELLOADER_H
#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include <memory>
#include <SDL2/SDL.h>
#include "../Logger/Logger.h"

class LevelLoader {

    public:
        LevelLoader();
        ~LevelLoader();

        void LoadLevel(std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, int level);


};

#endif