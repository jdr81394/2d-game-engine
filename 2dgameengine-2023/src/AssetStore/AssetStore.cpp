#include "AssetStore.h"
#include "../Logger/Logger.h"
#include <SDL_image.h>

AssetStore::AssetStore() {
    Logger::Log("AssetStore constructer called!");
};

AssetStore::~AssetStore() {
    ClearAssets();
    Logger::Log("AssetStore destructor called!");
};

void AssetStore::ClearAssets() {
    for(auto texture : textures ) {
        SDL_DestroyTexture(texture.second);     // because its a map and the value not hte key that must be destroyed
    };
    textures.clear();

    for(auto font : fonts) {
        TTF_CloseFont(font.second);             // The equivalent of SDL_DestroyTexture() for fonts
    };

    fonts.clear();
};

SDL_Texture* AssetStore::GetTexture(const std::string& assetId)  {
    return textures[assetId];
};

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Created Texture from surface. Now we can free that surface
    SDL_FreeSurface(surface);

    // Add the texture to the map
    textures.emplace(assetId, texture);

    Logger::Log("New texture added to the Asset Store with id = "  + assetId);

};


void AssetStore::AddFont(const std::string& assetId, const std::string& filePath, int fontSize) {
    fonts.emplace(
        assetId, 
        TTF_OpenFont(filePath.c_str(), fontSize)
    );

};

TTF_Font* AssetStore::GetFont(const std::string& assetId) {
    return fonts[assetId];
}


