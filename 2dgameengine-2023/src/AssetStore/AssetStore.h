#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <SDL.h>
#include <map>
#include <string>
#include <SDL_ttf.h>
#include <iostream>


class AssetStore {
    private:
        std::map<std::string, SDL_Texture*> textures;
        std::map<std::string, TTF_Font*> fonts;
    public:
        void ClearAssets();
        void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath, bool isWorldEditor);
        std::map<std::string, SDL_Texture*> GetAllTextures() {
            return textures;
        };
        void AddTextureDirectly(const std::string& assetId, SDL_Texture* texture) {
            textures.emplace(assetId, texture);
        }
        SDL_Texture* GetTexture(const std::string& assetId);

        void AddFont(const std::string& assetId, const std::string& filePath, int fontSize);
        TTF_Font* GetFont(const std::string& assetId);
        SDL_Texture* CopyTexture(SDL_Renderer* renderer, SDL_Texture* sourceTexture, SDL_Window* window);

        AssetStore();
        ~AssetStore();

};

#endif