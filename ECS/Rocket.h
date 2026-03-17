#ifndef ROCKET_H
#define ROCKET_H
#include "components.h"
#include "../game.h"
#include "Inventory.h"
#include <string>
#include "../Text.h"
#include "TransformComponent.h"

class Rocket
{
    public:

        std::string name = "rocket";
        std::unique_ptr<Text> totalThrusterAccelerationText;
        Entity* entity = nullptr;
        float totalThrust = 0.0f;
        int prevThrusterCount = 0;
        int thrusterCount = 0;
        
        Rocket(const char* rocket_name)
        {
            name = rocket_name;
            init();
        }
        ~Rocket(){}

        void init() 
        {
            totalThrusterAccelerationText = std::make_unique<Text>("assets/fonts/Inter.ttf","Thrust Acceleration: 0",15, SDL_Color({0,0,0, 255}), 10, 40, Game::renderer);
            if(entity!=nullptr){
                if(entity->hasComponent<Inventory>()){
                    auto inventory = &entity->getComponent<Inventory>();
                    thrusterCount = (int)inventory->thrusters.size();
                }
            }
        }

        void update()
        {

                
                if(entity!=nullptr)
                {
                    if(entity->hasComponent<TransformComponent>()){
                        auto transform = &entity->getComponent<TransformComponent>();

                    
                            
                        
                        if(entity->hasComponent<Inventory>()){
                            auto inventory = &entity->getComponent<Inventory>();

                            inventory->thrusters.erase(
                                std::remove_if(inventory->thrusters.begin(), inventory->thrusters.end(),
                                    [](Entity* e){ return e == nullptr || !e->isActive(); }),
                                inventory->thrusters.end()
                            );

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
                                    for(Entity* t : inventory->thrusters)
                                    {
                                        t->getComponent<TransformComponent>().width = thrusterWidth;
                                        t->getComponent<TransformComponent>().height = thrusterHeight;
                                        //t->getComponent<SpriteComponent>().srcRect.w = thrusterWidth;
                                        //t->getComponent<SpriteComponent>().srcRect.h = thrusterHeight;

                                        t->getComponent<TransformComponent>().position.x = prevThrusterPos.x == 0.0f ? t->getComponent<TransformComponent>().position.x: prevThrusterPos.x + thrusterWidth;
                                        

                                        prevThrusterPos.x = t->getComponent<TransformComponent>().position.x;
                                        prevThrusterPos.y = t->getComponent<TransformComponent>().position.y;

                                    }
                                    break;
                                default:
                                    break;
                                }
                            }
                            prevThrusterCount = thrusterCount;
                            
                            float netThrust = 0.0f;
                            for(auto& tEntity : inventory->thrusters)
                            {
                                if(tEntity == nullptr) continue;
                                auto& thruster = tEntity->getComponent<Thruster>();
                                if(thruster.active){
                                    netThrust += thruster.thrust;
                                }
                            }
                            totalThrust = netThrust;
                        }
                        float totalThrustAcceleration = totalThrust;

                        //actual accelaration
                        //float totalThrustAcceleration = totalThrust / transform->mass;
                        totalThrusterAccelerationText->setText("Thrust Acceleration: " + std::to_string(totalThrustAcceleration), SDL_Color({0,0,0, 255}));
                    }
                }
                
        }

        void draw()
        {
           if(totalThrusterAccelerationText) totalThrusterAccelerationText->render();
        }
};




#endif