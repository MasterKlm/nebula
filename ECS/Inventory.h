#ifndef INVENTORY_H
#define INVENTORY_H


#include "components.h"
#include "Thruster.h"
#include <vector>
#include <memory>



class Inventory : public Component
{
    public:
        std::vector<Entity*> thrusters;
        int maxThrusters = 2;
       


        Inventory(){
            thrusters.reserve(maxThrusters); // reserve space for 3 thrusters
            
        }

        void add(const char* tag, Entity* thrusterEntity){
             if(strcmp(tag, "thrusters")== 0){
                if(thrusters.size() < (size_t)maxThrusters){
                    thrusters.push_back(thrusterEntity);
                }
             }
                
              
        }

        ~Inventory(){
            
        }
};












#endif