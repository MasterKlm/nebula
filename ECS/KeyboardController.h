#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H
#include "ecs.h"
#include <SDL2/SDL.h>
#include "Inventory.h"
#include "Thruster.h"
#include "TransformComponent.h"
#include "../game.h"


class KeyboardController : public Component
{
    public:
        TransformComponent* transform = nullptr;
        float thrustAcceleration = 0.0f;
   

        void init() override
        {
                if (entity->hasComponent<TransformComponent>() && transform == nullptr) {
                    transform = &entity->getComponent<TransformComponent>();
                }
                if(entity->hasComponent<Thruster>()){
                    entity->getComponent<Thruster>().active = true;
                    thrustAcceleration = entity->getComponent<Thruster>().thrust / transform->mass;
                }
        }

        void update() override
        {
            const Uint8* keystate = SDL_GetKeyboardState(NULL);
            if(keystate[SDL_SCANCODE_W])
            {
               if(entity->hasComponent<Thruster>()){
                        entity->getComponent<Thruster>().active = true;   
                }
                
                
                
                transform->velocity.y -= thrustAcceleration * Game::dt;


            }
            else{
                if(entity->hasComponent<Thruster>()){
                    entity->getComponent<Thruster>().active = false;
                }
            }
        }

      
};

#endif