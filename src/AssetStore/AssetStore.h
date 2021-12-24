#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <SDL2/SDL.h>
#include <map>
#include <string>


class AssetStore {
    private:
        std::map<std::string, SDL_Texture*> textures;
    public:
        void ClearAssets();
        // void AddTexture(SDL_Renderer* renderer, std::string& assetId, std::string& filePath);
        void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
        SDL_Texture* GetTexture(const std::string& assetId);
        // SDL_Texture* GetTexture(std::string assetId);
        AssetStore();
        ~AssetStore();

};

#endif