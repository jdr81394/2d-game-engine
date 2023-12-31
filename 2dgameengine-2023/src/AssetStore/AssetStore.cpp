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
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, surface->w, surface->h);

    // Created Texture from surface. Now we can free that surface
    SDL_FreeSurface(surface);

    // Add the texture to the map
    textures.emplace(assetId, texture);

    Logger::Log("New texture added to the Asset Store with id = "  + assetId);

};

SDL_Texture* AssetStore::CopyTexture(SDL_Renderer* renderer, SDL_Texture* sourceTexture, SDL_Window* window) {
    // Get the width and height of the source texture
    int width, height;
    SDL_QueryTexture(sourceTexture, NULL, NULL, &width, &height);

    // Create a new texture with the same format and access as the source texture
    SDL_Texture* destinationTexture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, width, height);

    // Set the destination texture as the render target
    SDL_SetRenderTarget(renderer, destinationTexture);

    // Clear the render target
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Copy the content of the source texture to the destination texture
    SDL_RenderCopy(renderer, sourceTexture, NULL, NULL);

    // Reset the render target to the default
    SDL_SetRenderTarget(renderer, NULL);

    return destinationTexture;
}


void AssetStore::AddFont(const std::string& assetId, const std::string& filePath, int fontSize) {
    fonts.emplace(
        assetId, 
        TTF_OpenFont(filePath.c_str(), fontSize)
    );

};

TTF_Font* AssetStore::GetFont(const std::string& assetId) {
    return fonts[assetId];
}


