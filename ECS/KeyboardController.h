#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H
#include "../game.h"
#include "ecs.h"
#include "components.h"



class KeyboardController : public Component
{
    public:
        TransformComponent* transform;


        void init() override
        {
            transform = &entity->getComponent<TransformComponent>();
        }



        void update() override
        {
            if(Game::event.type == SDL_KEYDOWN){
                switch(Game::event.key.keysym.sym)
                {   
                    case SDLK_w:
                        transform->velocity.y = transform->speed * Game::dt;
                        transform->position.y -= transform->velocity.y * Game::dt;
                        break;
                    default:
                        break;
                }
            }
            
        }
};



#endif