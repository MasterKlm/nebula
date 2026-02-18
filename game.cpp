#include "game.h"
#include "TextureManager.h"
#include "ECS/components.h"
#include "Text.h"
#include <cmath>


SDL_Renderer* Game::renderer = nullptr;
float Game::dt = 1.0f;
Manager manager;
SDL_Event Game::event;
Vector2d Game::camera;
int Game::WINDOW_WIDTH = 800;
int Game::WINDOW_HEIGHT = 600;

auto& rocket(manager.addEntity());
auto& n1Thruster(manager.addEntity());
auto& launchPad(manager.addEntity());


Game::Game()
{
    init("Nebula Physics | rocket simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, false);
}

Game::~Game()
{
    clean();
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flag = 0;
    if(fullscreen==true)
    {
        flag = SDL_WINDOW_FULLSCREEN;
    }


    if(SDL_Init(SDL_INIT_EVERYTHING)==0)
    {
        std::cout << "Subsystems loaded" << "\n";

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flag);
        if(window)
        {
            std::cout << "Window Created Successfully" << "\n";
            renderer = SDL_CreateRenderer(window, -1, 0);
            if(renderer)
            {
                std::cout << "Renderer Created Successfully" << "\n";
                SDL_SetRenderDrawColor(renderer, 255, 255, 255,255);
                        isRunning = true;

                        // create FPS text
                        fpsText = std::make_unique<Text>("assets/fonts/PixelifySans-Bold.ttf", "FPS: 0", 16, SDL_Color{0,0,0,255}, 10, 40, renderer);
            }
        }

    }
    else{
        isRunning = false;
    }
    launchPad.addComponent<TransformComponent>(0.0f, 500.0f, 800, 100, 2, 1.0f);
    launchPad.addComponent<SpriteComponent>("assets/floor.png");
    launchPad.addComponent<ColliderComponent>("launchPad");

    rocket.addComponent<TransformComponent>(400.0f, 400.0f, 100, 100, 1, 50.0f);
    Game::camera = Vector2d(400.0f, 400.0f);
    rocket.addComponent<SpriteComponent>("assets/nebularocket1.png");
    rocket.addComponent<ColliderComponent>("rocket");
    
    rocket.addComponent<Inventory>();

    n1Thruster.addComponent<Thruster>(N1Thruster(), "n1");
    n1Thruster.addComponent<TransformComponent>(
        rocket.getComponent<TransformComponent>().position.x + (float)rocket.getComponent<TransformComponent>().width/4,
        rocket.getComponent<TransformComponent>().position.y + (float)rocket.getComponent<TransformComponent>().height,
        rocket.getComponent<TransformComponent>().width/3,  rocket.getComponent<TransformComponent>().height/4, 1, n1Thruster.getComponent<Thruster>().mass 
    );
    n1Thruster.addComponent<SpriteComponent>("assets/n1thruster.png");
    n1Thruster.addComponent<ColliderComponent>("thuster");
    n1Thruster.addComponent<KeyboardController>();
    rocket.getComponent<Inventory>().add("thrusters", std::make_unique<Thruster>(n1Thruster.getComponent<Thruster>()));
}   

void Game::update()
{

    manager.refresh();
    manager.update();

 static Uint32 prevTicks = SDL_GetTicks();
    Uint32 now = SDL_GetTicks();
    float frameDt = (now - prevTicks) / 1000.0f;
    prevTicks = now;
    // update FPS display
    if (fpsText) {
        int fps = 0;
        if (frameDt > 0.0f) fps = (int)(1.0f / frameDt + 0.5f);
        fpsText->setText(std::string("FPS: ") + std::to_string(fps), SDL_Color{0,0,0,255});
    }
    if (frameDt <= 0.0f) frameDt = 1.0f/60.0f;
    if (frameDt > 0.25f) frameDt = 0.25f; // clamp huge frames

    const float fixedStep = 1.0f / 120.0f; // smaller steps reduce tunneling
    float remaining = frameDt;
    while (remaining > 0.0f) {
        dt = std::min(remaining, fixedStep);

        // integrate velocity and position
        for (auto& ePtr : manager.entities) {
            Entity* e = ePtr.get();
            if (e == &launchPad) continue; // skip pad
           
            
            if (e->hasComponent<TransformComponent>()) {
                auto& tc = e->getComponent<TransformComponent>();
                tc.acceleration = tc.mass * manager.gravity;
                tc.velocity.y +=  tc.acceleration * dt; // gravity -> velocity
                tc.position.y += tc.velocity.y * dt;             // velocity -> position
            }

            
            
        }

        // update colliders after moving
        for (auto& ePtr : manager.entities) {
            Entity* e = ePtr.get();
            if (e->hasComponent<ColliderComponent>()) {
                e->getComponent<ColliderComponent>().update();
            }
        }

        // per-step collision response
        for (auto& ePtr : manager.entities)
        {
            Entity* e = ePtr.get();
            if (e == &launchPad) continue; // don't test pad vs itself
            //for (auto& otherEPtr : manager.entities){
            //    Entity* otherE = otherEPtr.get();
            //    if(otherE == &e) continue;

                //if(Collision::AABB(e->getComponent<ColliderComponent>().collider, otherE->getComponent<ColliderComponent>().collider)){
                    //per entity collision with other entities
                //}
            //}
            
            if (launchPad.hasComponent<ColliderComponent>() && e->hasComponent<ColliderComponent>()) {
                if (Collision::AABB(e->getComponent<ColliderComponent>().collider,
                                     launchPad.getComponent<ColliderComponent>().collider)) {

                    // ensure the entity has a Transform before accessing it
                    if (!e->hasComponent<TransformComponent>() || !launchPad.hasComponent<TransformComponent>())
                        continue;

                    auto& rtc = e->getComponent<TransformComponent>();
                    auto& ltc = launchPad.getComponent<TransformComponent>();
                    // snap rocket on top of pad and stop vertical motion
                    rtc.position.y = ltc.position.y - (rtc.height * rtc.scale);
                    
                    if (rtc.velocity.y > 0) {  // if moving downward (positive Y is down)
                        rtc.velocity.y = 0;     // stop it
                    }
                }
            }
        }

        for (auto& thruster: rocket.getComponent<Inventory>().thrusters){
            auto thc = thruster.get()->entity->getComponent<ColliderComponent>();
                if( thruster.get()->entity->hasComponent<ColliderComponent>() && rocket.hasComponent<ColliderComponent>()){
                    if(Collision::AABB(thc.collider, rocket.getComponent<ColliderComponent>().collider)){
                    auto& rtc = rocket.getComponent<TransformComponent>();
                    auto& thtc = thruster.get()->entity->getComponent<TransformComponent>();
                    // snap rocket on top of thruster and stop vertical motion
                    rtc.position.y = thtc.position.y - (rtc.height * rtc.scale);
                    if(thruster.get()->entity->getComponent<Thruster>().active){
                        thruster.get()->entity->getComponent<TransformComponent>().velocity.y += ( manager.gravity / rtc.mass ) * dt;
                    }
                    if (rtc.velocity.y > 0) {  // if moving downward (positive Y is down)
                        rtc.velocity.y = 0;     // stop it
                    }
                    }
                }
            }
        Game::camera.x = rocket.getComponent<TransformComponent>().position.x - WINDOW_WIDTH/2;
        Game::camera.y = rocket.getComponent<TransformComponent>().position.y - WINDOW_HEIGHT/2;

        remaining -= dt;
    }

    //std::cout << "x: " << rocket.getComponent<TransformComponent>().position.x
    //          << ", y: " << rocket.getComponent<TransformComponent>().position.y << ", velocityY: " << rocket.getComponent<TransformComponent>().velocity.y << "\n";
   

}
void Game::handleEvents()
{
    SDL_PollEvent(&event);
    switch(event.type)
    {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }
}
void Game::render()
{
    SDL_RenderClear(renderer);
    

    manager.draw();
    if (fpsText) fpsText->render();

    SDL_RenderPresent(renderer);
}
bool Game::running()
{
    return isRunning;
}
void Game::clean()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    std::cout << "Cleaned game" << "\n";
}