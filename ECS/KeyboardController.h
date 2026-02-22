#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H
#include "../game.h"
#include "ecs.h"
#include "components.h"
#include "Inventory.h"
#include "../Text.h"
#include <memory>

class KeyboardController : public Component
{
    public:
        TransformComponent* transform;
        std::unique_ptr<Text> thrusterAccelerationText; // { changed code }

        void init() override
        {
            transform = &entity->getComponent<TransformComponent>();
            // create persistent Text to be drawn in draw()
            thrusterAccelerationText = std::make_unique<Text>("assets/fonts/PixelifySans-Regular.ttf",
                                                              "Thrust Acceleration: 0",
                                                              15, SDL_Color({0,0,0}), 10, 10, Game::renderer); // { changed code }
        }

        void update() override
        {
            const Uint8* keystate = SDL_GetKeyboardState(NULL);
            if(keystate[SDL_SCANCODE_W])
            {
                float totalThrust = 0.0f;
                if(entity->hasComponent<Thruster>()){
                    entity->getComponent<Thruster>().active = true;
                    totalThrust = entity->getComponent<Thruster>().thrust;
                }
                else if(entity->hasComponent<Inventory>()){
                    for(auto& thruster : entity->getComponent<Inventory>().thrusters){
                        totalThrust += thruster->thrust;
                    }
                }

                float thrustAcceleration = totalThrust / transform->mass;
                transform->velocity.y -= thrustAcceleration * Game::dt;

                // update persistent text (recreate texture with new string)
                thrusterAccelerationText = std::make_unique<Text>("assets/fonts/PixelifySans-Bold.ttf",
                                                                  "Thrust Acceleration: " + std::to_string(thrustAcceleration),
                                                                  15, SDL_Color({0,0,0}), 10, 10, Game::renderer); // { changed code }
            }
            else{
                if(entity->hasComponent<Thruster>()){
                    entity->getComponent<Thruster>().active = false;
                }
            }
        }

        void draw() override
        {
            if(thrusterAccelerationText) thrusterAccelerationText->render(); // { changed code }
        }
};

#endif