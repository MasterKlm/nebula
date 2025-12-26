#ifndef THRUSTER_H
#define THRUSTER_H

#include "components.h"


struct N1Thruster 
{
    float mass = 2.0f;
    float thrust = 4.0f;
};



class Thruster : public Component
{
    public:
        float thrust;
        float mass;
        Thruster()
        {
            thrust = 0;
            mass = 0;
        }
        ~Thruster(){
           
        }
        template <typename T>
        Thruster(T blueprint)
        {
            thrust = blueprint.thrust;
            mass = blueprint.mass;
        }
};















#endif