#ifndef THRUSTER_H
#define THRUSTER_H

#include "ecs.h"
#include <string>


struct N1Thruster 
{
    float mass = 2.0f;
    float thrust = 200.0f;
};

struct N2Thruster 
{
    float mass = 2.0f;
    float thrust = 300.0f;
};




class Thruster : public Component
{
    public:
        float thrust;
        float mass;
        bool active = false;

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
 
        }
};















#endif