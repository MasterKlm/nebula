#ifndef THRUSTER_H
#define THRUSTER_H

#include "components.h"


struct N1Thruster 
{
    float mass = 2.0f;
    float thrust = 200.0f;
};



class Thruster : public Component
{
    public:
        float thrust;
        float mass;
        bool active = false;
        std::string id; 
        Thruster()
        {
            thrust = 0.0f;
            mass = 0.0f;
        }
        ~Thruster(){
           
        }
        template <typename T>
        Thruster(T blueprint, const std::string thrusterId = "")
        {
            thrust = blueprint.thrust;
            mass = blueprint.mass;
            id = thrusterId; 
        }
};















#endif