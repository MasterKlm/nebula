#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include "ecs.h"
#include <SDL2/SDL.h>
#include "../Vector2d.h"

class TransformComponent : public Component
{
    public:

    Vector2d position = Vector2d(0.0f,0.0f);
    Vector2d velocity = Vector2d(1.0f,1.0f);

    int width = 100;
    int height = 100;
    int scale = 1;
    float mass = 1.0f;
    
    
    float acceleration = 0.0f;
    Uint32 lastTicks = SDL_GetTicks();
    Vector2d prevPos;

    TransformComponent(){
        prevPos = position;
    }
    TransformComponent(int sc)
    {
        scale = sc;
        prevPos = position;

    }
    TransformComponent(float x, float y)
    {
        position.x = x;
        position.y = y;
        prevPos = position;
    }
    TransformComponent(int w, int h)
    {
        width = w;
        height = h;
        prevPos = position;
    }
    TransformComponent(float x, float y, int w, int h, int sc, float m)
    {
        position.x = x;
        position.y = y;
        width = w;
        height = h;
        scale = sc; 
        mass = m;
        prevPos = position;

    }
    
        void init() override
        {

        }
        void update() override
        {
            // Transform is a pure data component; camera offset is applied by render components.
        }
};














#endif