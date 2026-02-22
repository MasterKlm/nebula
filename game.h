#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Collision.h"
#include "Vector2d.h"
#include "Text.h"
#include <chrono>
#include <thread>
#include <memory>
#include <atomic>



using namespace std::chrono;

class Game
{
    public:
    static SDL_Renderer* renderer;
    static SDL_Event event;
    static Vector2d camera;
    static float dt;
    static int WINDOW_WIDTH;
    static int WINDOW_HEIGHT;
    int launchTime = 10;
    bool launched = false;
    bool timerStarted = false;
    std::atomic<int> timerCount{-1};
    

    Game();
    ~Game();


    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void update();
    void handleEvents();
    void render();
    bool running();
    void clean();
    void startTimer();

    private:
    bool isRunning;
    SDL_Window* window;
    std::unique_ptr<Text> fpsText;
    
};




#endif