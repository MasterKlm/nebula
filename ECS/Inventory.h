#ifndef INVENTORY_H
#define INVENTORY_H


#include "components.h"
#include "Thruster.h"
#include <vector>
#include <memory>



class Inventory : public Component
{
    public:
        std::vector<std::unique_ptr<Thruster>> thrusters;
        int maxThrusters = 3;
       


        Inventory(){
            thrusters.reserve(maxThrusters); // reserve space for 3 thrusters
            
        }

        template <typename T>
        void add(const char* tag, std::unique_ptr<T> component_ptr){
             if(strcmp(tag, "thrusters")== 0){
                if(thrusters.size() < maxThrusters){
                    thrusters.push_back(std::move(component_ptr));
                }
             }
                
              
        }

        ~Inventory(){
            
        }
};












#endif