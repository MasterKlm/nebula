#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Collision.h"

class Game
{
    public:
    static SDL_Renderer* renderer;
    static SDL_Event event;
    static float dt;

    Game();
    ~Game();


    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void update();
    void handleEvents();
    void render();
    bool running();
    void clean();

    private:
    bool isRunning;
    SDL_Window* window;
};




#endif