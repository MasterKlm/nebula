#ifndef COLLISION_H
#define COLLISION_H
#include <SDL2/SDL.h>


class Collision
{
    public:
        static bool AABB(const SDL_Rect& recA, const SDL_Rect& rectB);
};

#endif