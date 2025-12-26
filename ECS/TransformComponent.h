#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include "components.h"
#include "../Vector2d.h"


class TransformComponent : public Component
{
    public:

    Vector2d position = Vector2d(0.0f,0.0f);
    Vector2d velocity = Vector2d(1.0f,1.0f);

    int width = 100;
    int height = 100;
    int scale = 1;
    float speed = 4.0f;
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
    TransformComponent(float x, float y, int w, int h, int sc)
    {
        position.x = x;
        position.y = y;
        width = w;
        height = h;
        scale = sc; 
        prevPos = position;

    }
    
        void init() override
        {

        }
        void update() override
        {
            
            Uint32 now = SDL_GetTicks();
            float dt = (now - lastTicks) / 1000.0f;
            if (dt <= 0.0f) dt = 1.0f/60.0f; // fallback

            Vector2d distance(position.x - prevPos.x, position.y - prevPos.y);
            Vector2d newVelocity(distance.x / dt, distance.y / dt);
            
            
            velocity = newVelocity;
            

            lastTicks = now;
            prevPos = position; 
        
        }
};














#endif