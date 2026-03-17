#include "game.h"
#include "ECS/components.h"
#include "ECS/TransformComponent.h"
#include "ECS/KeyboardController.h"
#include "ECS/SpriteComponent.h"
#include "ECS/Rocket.h"
#include "ECS/ColliderComponent.h"
#include "ECS/Thruster.h"
#include "ECS/Inventory.h"
#include "Text.h"
#include <cmath>
#include <cstring>



SDL_Renderer* Game::renderer = nullptr;
float Game::dt = 1.0f;
Manager* manager = new Manager();
SDL_Event Game::event;
Vector2d Game::camera;
int Game::WINDOW_WIDTH = 800;
int Game::WINDOW_HEIGHT = 600;
Text* launchTimerText = nullptr;
Text* liftOffText = nullptr;
Text* gravityText = nullptr;
auto& sky_box(manager->addEntity());
Rocket* rocket = nullptr;
Entity* firstThruster;
auto& launchPad(manager->addEntity());
Entity* secondThruster;
inline ImGuiIO* io = nullptr;
bool uiOpen = true;
constexpr float camera_x = 0.0f;
constexpr float camera_y = 100.f;

static char buffer_Rocket_Name[64] = "";
static const char* thrusterOptions[] = {"none","n1 thruster", "n2 thruster"};
static int selectedThruster1Index = 0;
static int selectedThruster2Index = 0;

//fonts
ImFont* poppins = nullptr;
ImFont* inter = nullptr;

Game::Game()
{
    init("Nebula Physics | rocket simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT);
}

Game::~Game()
{
    clean();
}

void Game::init(const char* title, int xpos, int ypos, int width, int height)
{
    // Remove SDL_WINDOW_OPENGL flag - no longer needed
    SDL_WindowFlags flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    #ifdef _WIN32
        ::SetProcessDPIAware();
    #endif

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        TTF_Init();
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if(window)
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
            if(renderer)
            {
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                io = &ImGui::GetIO();
                io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
                ImGui::StyleColorsDark();

                ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
                ImGui_ImplSDLRenderer2_Init(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                isRunning = true;
                fpsText = new Text("assets/fonts/Inter.ttf", "FPS: 0", 16, SDL_Color{0,0,0,255}, 10, 70, renderer);

                poppins = io->Fonts->AddFontFromFileTTF("assets/fonts/Poppins-ExtraBold.ttf");
                inter = io->Fonts->AddFontFromFileTTF("assets/fonts/Inter.ttf");
            }
        }
    }
    else { isRunning = false; }

    launchPad.addComponent<TransformComponent>(0.0f, 500.0f, 800, 100, 2, 1.0f);
    launchPad.addComponent<SpriteComponent>("assets/floor.png");
    launchPad.addComponent<ColliderComponent>("launchPad");

    sky_box.addComponent<TransformComponent>(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1, 1.0f);
    sky_box.addComponent<SpriteComponent>("assets/sky_box.png");
    

    Game::camera = Vector2d(camera_x, camera_y);
    gravityText = new Text("assets/fonts/Inter.ttf", "Gravity: " + std::to_string(manager->gravity), 16, SDL_Color{0,0,0,255}, 10, 100, renderer);

    
    

}

void Game::createRocket(const char* rocket_name){
    if(rocket!=nullptr) return;
    rocket = new Rocket(rocket_name);
    rocket->entity = &manager->addEntity();
    if(rocket->entity==nullptr) return;
    float rocket_x = 400.0f;
    float rocket_y = 300.0f;
    rocket->entity->addComponent<TransformComponent>(rocket_x, rocket_y, 100, 100, 1, 50.0f);
    Game::camera.x = rocket_x;
    Game::camera.y = rocket_y;
    rocket->entity->addComponent<SpriteComponent>("assets/nebularocket1.png");
    rocket->entity->addComponent<ColliderComponent>("rocket"); 
    rocket->entity->addComponent<Inventory>();

    launchTimerText = new Text("assets/fonts/Poppins-ExtraBold.ttf", std::string("Launching ") + rocket->name + std::string(" in: ") + std::to_string(launchTime), 16, SDL_Color{0,0,0,150}, WINDOW_WIDTH/2, 10, renderer);
    
}

void Game::destroyRocket()
{
    if(rocket!=nullptr){
        rocket = nullptr;
    }
}

void Game::addThruster(const char* tag, int thrustIndex)
{
    if(tag == nullptr || rocket == nullptr) return;
    if(std::strcmp(tag, "none") == 0) return;

    Entity*& targetSlot = (thrustIndex == 0) ? firstThruster : secondThruster;

    // clean up old thruster
    if(targetSlot != nullptr){
        targetSlot->destroy();
        targetSlot = nullptr;

    }

    // create new thruster entity
    targetSlot = &manager->addEntity();

    if(std::strcmp(tag, "n1 thruster") == 0)
        targetSlot->addComponent<Thruster>(N1Thruster());
    else if(std::strcmp(tag, "n2 thruster") == 0)
        targetSlot->addComponent<Thruster>(N2Thruster());

    auto& rtc = rocket->entity->getComponent<TransformComponent>();
    targetSlot->addComponent<TransformComponent>(
        rtc.position.x + rtc.width / 4.0f,
        rtc.position.y + rtc.height,
        rtc.width / 3, rtc.height / 4,
        1, targetSlot->getComponent<Thruster>().mass
    );
    targetSlot->addComponent<SpriteComponent>("assets/n1thruster.png");
    targetSlot->addComponent<ColliderComponent>(thrustIndex == 0 ? "firstThruster" : "secondThruster");
    targetSlot->addComponent<KeyboardController>();

    rocket->entity->getComponent<Inventory>().add("thrusters", targetSlot);
}
void Game::showMenu(){
    if(poppins && inter){
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(ImVec2(250,Game::WINDOW_HEIGHT));
        ImGui::Begin("##1", &uiOpen);
        ImGui::PushFont(inter);
        ImGui::Text("Rocket Name: ");
        ImGui::InputText("##2", buffer_Rocket_Name, sizeof(buffer_Rocket_Name));
        ImGui::PopFont();
        ImGui::PushFont(poppins);
        if(ImGui::Button("Build rocket"))
        {
            createRocket(buffer_Rocket_Name);
        }
        ImGui::PopFont();

        ImGui::PushFont(inter);
        // guard against null rocket
        if(rocket != nullptr && rocket->entity != nullptr)
        {
            if(rocket->entity->hasComponent<Inventory>())
            {
                auto& inventory = rocket->entity->getComponent<Inventory>();
                // use std::to_string to build the string properly

                ImGui::Text("Thrusters: %d / %d", 
                    (int)inventory.thrusters.size(), 
                    inventory.maxThrusters);
                
                if(ImGui::Combo("##3", &selectedThruster1Index, thrusterOptions, IM_ARRAYSIZE(thrusterOptions))){
                    addThruster(thrusterOptions[selectedThruster1Index],0);
                }
                
                if(ImGui::Combo("##4", &selectedThruster2Index, thrusterOptions, IM_ARRAYSIZE(thrusterOptions))){
                    addThruster(thrusterOptions[selectedThruster2Index], 1);
                }
            }
        }
        ImGui::PopFont();


        ImGui::End();
    }
}


void Game::startTimer()
{
    for(int i = launchTime; i >= 0; i--){
        timerCount = i;
        std::this_thread::sleep_for(1s);
    }
    launched = true;
}

void Game::update()
{
    if(isRunning == false) return;
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if(keystate[SDL_SCANCODE_ESCAPE])
    {
        
        setIsRunning(false);
    }

    //update rocket name
    if(rocket!=nullptr)
    {
        if(rocket->name != buffer_Rocket_Name){
            rocket->name = buffer_Rocket_Name;
        }
    }
    

    manager->refresh();
    if(rocket!=nullptr){
        rocket->update();
    }
    manager->update();

    static Uint32 prevTicks = SDL_GetTicks();
    Uint32 now = SDL_GetTicks();
    float frameDt = (now - prevTicks) / 1000.0f;
    prevTicks = now;

   
    if(timerCount >= 0 && launchTimerText){
        launchTimerText->setText("Launching " + rocket->name + std::string(" in: ") + std::to_string(timerCount.load()), SDL_Color{0,0,0,150});
    }   

    if(launched && launchTimerText){
        delete launchTimerText;
        launchTimerText = nullptr;

        liftOffText = new Text("assets/fonts/Inter.ttf", "Liftoff! Press W to engage Thrusters", 15, SDL_Color{0,0,0,255}, WINDOW_WIDTH/3, 10, renderer);
    }
   
    if(launchTimerText && !launched && !timerStarted){
        timerStarted = true;
        std::thread startTimerWorker(&Game::startTimer, this);
        startTimerWorker.detach();
    }
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
        for (auto& ePtr : manager->entities) {
            Entity* e = ePtr.get();
            if (e == &launchPad) continue; // skip gravity
            if (e == &sky_box) continue; // skip gravity
           
            
            if (e->hasComponent<TransformComponent>()) {
                auto& tc = e->getComponent<TransformComponent>();
                tc.acceleration = tc.mass * manager->gravity;
                tc.velocity.y +=  tc.acceleration * dt; // gravity -> velocity
                tc.position.y += tc.velocity.y * dt;             // velocity -> position
                
            }

            
            
        }

        // update colliders after moving
        for (auto& ePtr : manager->entities) {
            Entity* e = ePtr.get();
            if (e->hasComponent<ColliderComponent>()) {
                e->getComponent<ColliderComponent>().update();
            }
        }

        // per-step collision response
        for (auto& ePtr : manager->entities)
        {
            Entity* e = ePtr.get();
            if (e == &launchPad) continue; // don't test pad vs itself
            //for (auto& otherEPtr : manager->entities){
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

        if(rocket!=nullptr){
            if(rocket->entity->hasComponent<Inventory>()){
            for (auto& thruster: rocket->entity->getComponent<Inventory>().thrusters){
            auto thc = thruster->getComponent<ColliderComponent>();
                if( thruster->hasComponent<ColliderComponent>() && rocket->entity->hasComponent<ColliderComponent>()){
                    if(Collision::AABB(thc.collider, rocket->entity->getComponent<ColliderComponent>().collider)){
                    auto& rtc = rocket->entity->getComponent<TransformComponent>();
                    auto& thtc = thruster->getComponent<TransformComponent>();
                    // snap rocket on top of thruster and stop vertical motion
                    rtc.position.y = thtc.position.y - (rtc.height * rtc.scale);
                    if(thruster->getComponent<Thruster>().active){
                        thruster->getComponent<TransformComponent>().velocity.y += ( manager->gravity / rtc.mass ) * dt;
                    }
                    if (rtc.velocity.y > 0) {  // if moving downward (positive Y is down)
                        rtc.velocity.y = 0;     // stop it
                    }
                    }
                }
            }
                Game::camera.x = rocket->entity->getComponent<TransformComponent>().position.x - WINDOW_WIDTH/2;
                Game::camera.y = rocket->entity->getComponent<TransformComponent>().position.y - WINDOW_HEIGHT/2;

            }
        }
        
        remaining -= dt;

        }

    //std::cout << "x: " << rocket.getComponent<TransformComponent>().position.x
    //          << ", y: " << rocket.getComponent<TransformComponent>().position.y << ", velocityY: " << rocket.getComponent<TransformComponent>().velocity.y << "\n";
   

}
void Game::handleEvents()
{
    SDL_PollEvent(&event);
    ImGui_ImplSDL2_ProcessEvent(&event);
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
    if(isRunning == false) return;

    // 1. Start ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    showMenu();

    // 2. Clear and draw game
    SDL_RenderClear(renderer);
    manager->draw();
    if(rocket!=nullptr){
        rocket->draw();
    }
    if (fpsText) fpsText->render();
    if (launchTimerText) launchTimerText->render();
    if (liftOffText) liftOffText->render();
    if (gravityText) gravityText->render();

    // 3. ImGui on top
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

    // 4. Present
    SDL_RenderPresent(renderer);
}

bool Game::running()
{
    return isRunning;
}
void Game::setIsRunning(bool value){
    isRunning = value;
}
void Game::clean()
{
    if(rocket) delete rocket;
    if(fpsText) delete fpsText;
    if(liftOffText) delete liftOffText;
    if(gravityText) delete gravityText;
    if(launchTimerText) delete launchTimerText;
    if(manager) delete manager;


    TTF_Quit();
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();


    std::cout << "Cleaned game" << "\n";
}