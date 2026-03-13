#ifndef ROCKET_H
#define ROCKET_H
#include "components.h"
#include "../game.h"
#include "Inventory.h"
#include <string>
#include "../Text.h"
#include "TransformComponent.h"

class Rocket : public Component
{
    public:

        std::string tag = "rocket";
        std::unique_ptr<Text> totalThrusterAccelerationText;
        float totalThrust = 0.0f;
        int prevThrusterCount = 0;
        int thrusterCount = 0;
        
        Rocket(std::string tag_name)
        {
            tag = tag_name;
        }
        ~Rocket(){}

        void init() override
        {
            totalThrusterAccelerationText = std::make_unique<Text>("assets/fonts/PixelifySans-Regular.ttf","Thrust Acceleration: 0",15, SDL_Color({0,0,0, 255}), 10, 10, Game::renderer);
            if(entity && entity->hasComponent<Inventory>()){
                auto inventory = &entity->getComponent<Inventory>();
                thrusterCount = (int)inventory->thrusters.size();
            }
        }

        void update() override
        {
                if(entity->hasComponent<TransformComponent>()){
                    auto transform = &entity->getComponent<TransformComponent>();

                  
                        
                    
                    if(entity->hasComponent<Inventory>()){
                        auto inventory = &entity->getComponent<Inventory>();

                        thrusterCount = (int)inventory->thrusters.size();
                        

                        if(thrusterCount != prevThrusterCount)
                        {
                            Vector2d prevThrusterPos = Vector2d(0.0f, 0.0f);
                            int thrusterWidth = 20;
                            int thrusterHeight = 20;

                            switch(thrusterCount){
                            case 1:
                                break;
                            case 2:
                                for(auto& t : inventory->thrusters)
                                {
                                    t->entity->getComponent<TransformComponent>().width = thrusterWidth;
                                    t->entity->getComponent<TransformComponent>().height = thrusterHeight;
                                    //t->entity->getComponent<SpriteComponent>().srcRect.w = thrusterWidth;
                                    //t->entity->getComponent<SpriteComponent>().srcRect.h = thrusterHeight;

                                    t->entity->getComponent<TransformComponent>().position.x = prevThrusterPos.x == 0.0f ? t->entity->getComponent<TransformComponent>().position.x: prevThrusterPos.x + thrusterWidth;
                                    

                                    prevThrusterPos.x = t->entity->getComponent<TransformComponent>().position.x;
                                    prevThrusterPos.y = t->entity->getComponent<TransformComponent>().position.y;

                                }
                                break;
                            default:
                                break;
                            }
                        }
                        prevThrusterCount = thrusterCount;
                        
                        float netThrust = 0.0f;
                        for(auto& t : inventory->thrusters)
                        {
                            if(t->active){
                                netThrust += t->thrust;
                            }
                        }
                        totalThrust = netThrust;
                    }
                    float totalThrustAcceleration = totalThrust / transform->mass;
                    totalThrusterAccelerationText->setText("Thrust Acceleration: " + std::to_string(totalThrustAcceleration), SDL_Color({0,0,0, 255}));
                }
        }

        void draw() override
        {
           if(totalThrusterAccelerationText) totalThrusterAccelerationText->render();
        }
};




#endif