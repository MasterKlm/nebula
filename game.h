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
#include <atomic>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_sdlrenderer2.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif
#ifdef _WIN32
#include <windows.h>        // SetProcessDPIAware()
#endif

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
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    SDL_GLContext gl_context = nullptr;
    

    Game();
    ~Game();


    void init(const char* title, int xpos, int ypos, int width, int height);
    void update();
    void handleEvents();
    void render();
    bool running();
    void setIsRunning(bool value);
    void clean();
    void startTimer();
    void showMenu();
    void createRocket(const char* rocket_name);
    void destroyRocket();
    void addThruster(const char* tag, int thrustIndex);

    private:
    bool isRunning;
    SDL_Window* window;
    Text* fpsText = nullptr;
    
};




#endif