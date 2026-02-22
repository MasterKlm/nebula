#include "game.h"
#include <SDL2/SDL_ttf.h>

#define CELL_SIZE 32

Game* game = nullptr;

int main(int args, char* argv[])
{
    
    TTF_Init();
    game = new Game();
    const int FPS = 60;

    const int frameDelay = 1000/FPS;
    Uint32 frameStart;
    int frameTime;

    while(game->running()){
        frameStart = SDL_GetTicks();

        //updates
        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;
        
        if(frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }


    }

    TTF_Quit();
    

    delete game;

    return 0;
}