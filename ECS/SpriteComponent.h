#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H
#include "ecs.h"
#include <SDL2/SDL.h>
#include "../TextureManager.h"
#include "TransformComponent.h"
#include "../game.h"


class SpriteComponent : public Component
{
    public:
        SDL_Rect srcRect, destRect;
        

        SpriteComponent() = default;
        SpriteComponent(const char* path)
        {
            setTexture(path);
        }
        ~SpriteComponent() override
        {
            SDL_DestroyTexture(texture);
            std::cout << "Deleted 1 sprite" << "\n";
        }
        void setTexture(const char* path)
        {
            texture = TextureManager::LoadTexture(path);
        }
        void init() override
        {
            transform = &entity->getComponent<TransformComponent>();
            srcRect.x = srcRect.y = 0;
            srcRect.w = transform->width;
            srcRect.h = transform->height;

            
        }

        void update() override
        {
            destRect.x = (int)(transform->position.x - Game::camera.x);
            destRect.y = (int)(transform->position.y - Game::camera.y);
            destRect.w = transform->width * transform->scale;
            destRect.h = transform->height * transform->scale;
        }

        void draw() override
        {
            TextureManager::Draw(texture, srcRect, destRect);
        }

    private:
        TransformComponent *transform;
        SDL_Texture *texture;



};


#endif