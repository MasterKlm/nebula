#include "TextureManager.h"
#include "game.h"

SDL_Texture* TextureManager::LoadTexture(const char* fileName)
{
    SDL_Surface* tmpSurface = IMG_Load(fileName);
    if(!tmpSurface)
    {
        std::cout << "Failed to load image: " << fileName << ", Error: " << IMG_GetError() << "\n";
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect srcRect, SDL_Rect destRect)
{
    SDL_RenderCopy(Game::renderer, tex, &srcRect, &destRect);
}