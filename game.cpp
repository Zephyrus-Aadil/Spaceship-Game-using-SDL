#include <SDL2/SDL.h>    
#include <SDL2/SDL_image.h>
#include <iostream>
const int WIDTH = 800, HEIGHT = 600;

SDL_Point velocity = { 0, 0 };
double angle = 0.0;
Uint64 current_time = SDL_GetPerformanceCounter();
Uint64 last_time = 0;
double delta_time = 0;
SDL_Rect destShip = {.x = WIDTH/2 -60,.y = HEIGHT/2 -60,.w = 120 ,.h = 120};

double get_delta_time() {
    last_time = current_time;
    current_time = SDL_GetPerformanceCounter();
    delta_time = static_cast<double>(current_time - last_time) / SDL_GetPerformanceFrequency();

    return delta_time;
}
void err(std::string error) {
    std::cout <<error<<"\n";
    exit(0);
}
void Game_init() {
    std::cout <<"Initializing SDL\n";
    if ( SDL_Init(SDL_INIT_VIDEO) < 0) {
        err("Error in opening SDL");
    }
}
void give_boost(float boost_amount) {
    velocity.x = boost_amount * cos(angle * 3.14159/180) ;
    velocity.y = boost_amount * sin(angle* 3.14159/180);
}
void update_spaceship(float dt) {
    
    SDL_Point new_pos = {
        (destShip.x + static_cast<int>(velocity.x * dt)) % WIDTH,
        (destShip.y + static_cast<int>(velocity.y * dt)) % HEIGHT
    };

    destShip.x = new_pos.x;
    destShip.y = new_pos.y;
}

int main (int argc, char *argv[]){
    Game_init();
    std::cout <<"Sucessfully Initialized SDL\n";
    
    SDL_Window * screen = NULL;
    SDL_Renderer * render = NULL;
    SDL_Surface *tempSurface = NULL;
    SDL_Texture * texture = NULL;

    SDL_Rect srcShip = {.x = 0,.y = 0,.w = 90 ,.h = 90};
    
    int imgFlags = IMG_INIT_PNG;
    bool isRunning = true;
    screen = SDL_CreateWindow("Game1",30,0,WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
    
    if ( screen == NULL){
    
        err("Error in Creating Window");
    }

    
    if(!(IMG_Init(imgFlags) & imgFlags)){
        err("Error in Image"); 
    }

    render = SDL_CreateRenderer(screen,-1,0);   
    tempSurface = IMG_Load("ship.png");
    texture = SDL_CreateTextureFromSurface(render,tempSurface);
    
    SDL_FreeSurface(tempSurface);
    SDL_SetRenderDrawColor(render,0,0,0,255);
    SDL_RenderPresent(render);
    bool boostGiven = false;
    while(isRunning) {
        Uint32 frameStart = SDL_GetTicks();
        float dt = get_delta_time();
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
                break;
            }
            if (event.key.keysym.sym == SDLK_d) {   
                angle += 3.5;
            }
            if (event.key.keysym.sym == SDLK_a) {
                angle -= 3.5;
            }
            if (event.key.keysym.sym == SDLK_w) {

                boostGiven  = true;
            }
        }
        if (boostGiven) {
            give_boost(200);
            boostGiven = false;
        }
        
        update_spaceship(dt);
        SDL_RenderClear(render);
        SDL_RenderCopyEx(render, texture, &srcShip ,&destShip, angle, nullptr, SDL_FLIP_NONE);
        SDL_RenderPresent(render);
        Uint32 frameEnd = SDL_GetTicks();
        if (frameEnd -frameStart <16) {
            SDL_Delay(16-(frameEnd-frameStart));
        }

    }
    std::cout <<"QUITTING\n";
    SDL_Quit();
    return 0;

}
/*
#include <SDL2/SDL.h>    
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cmath>

const int WIDTH = 800, HEIGHT = 600;
const float SHIP_ACCELERATION = 200.0f;
const float SHIP_MAX_SPEED = 300.0f;
const float SHIP_ROTATION_SPEED = 180.0f;

SDL_Point velocity = { 0, 0 };
double angle = 0.0;
Uint64 current_time = SDL_GetPerformanceCounter();
Uint64 last_time = 0;
double delta_time = 0;
SDL_Rect destShip = {.x = WIDTH/2 -60,.y = HEIGHT/2 -60,.w = 120 ,.h = 120};

double get_delta_time() {
    last_time = current_time;
    current_time = SDL_GetPerformanceCounter();
    delta_time = static_cast<double>(current_time - last_time) / SDL_GetPerformanceFrequency();

    return delta_time;
}

void err(std::string error) {
    std::cout <<error<<"\n";
    exit(0);
}

void Game_init() {
    std::cout <<"Initializing SDL\n";
    if ( SDL_Init(SDL_INIT_VIDEO) < 0) {
        err("Error in opening SDL");
    }
}

void give_boost(float boost_amount) {
    float acceleration_x = boost_amount * cos(angle);
    float acceleration_y = boost_amount * sin(angle);

    velocity.x += acceleration_x;
    velocity.y += acceleration_y;

    float speed = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
    if (speed > SHIP_MAX_SPEED) {
        velocity.x = SHIP_MAX_SPEED * cos(angle);
        velocity.y = SHIP_MAX_SPEED * sin(angle);
    }
}

void update_spaceship(float dt) {
    float acceleration_x = 0.0f;
    float acceleration_y = 0.0f;

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_W]) {
        acceleration_x = SHIP_ACCELERATION * cos(angle);
        acceleration_y = SHIP_ACCELERATION * sin(angle);
    }
    if (state[SDL_SCANCODE_S]) {
        acceleration_x = -SHIP_ACCELERATION * cos(angle);
        acceleration_y = -SHIP_ACCELERATION * sin(angle);
    }

    velocity.x += acceleration_x * dt;
    velocity.y += acceleration_y * dt;

    float speed = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
    if (speed > SHIP_MAX_SPEED) {
        velocity.x = SHIP_MAX_SPEED * cos(angle);
        velocity.y = SHIP_MAX_SPEED * sin(angle);
    }

    SDL_Point new_pos = {
        destShip.x + static_cast<int>(velocity.x * dt),
        destShip.y + static_cast<int>(velocity.y * dt)
    };

    destShip.x = new_pos.x;
    destShip.y = new_pos.y;

    if (state[SDL_SCANCODE_A]) {
        angle -= SHIP_ROTATION_SPEED * dt;
    }
    if (state[SDL_SCANCODE_D]) {
        angle += SHIP_ROTATION_SPEED * dt;
    }
}



int main(int argc, char* argv[]) {
    Game_init();
    std::cout << "Successfully Initialized SDL\n";

    SDL_Window* screen = NULL;
    SDL_Renderer* render = NULL;
    SDL_Surface* tempSurface = NULL;
    SDL_Texture* texture = NULL;

    SDL_Rect srcShip = { .x = 0,.y = 0,.w = 90,.h = 90 };

    int imgFlags = IMG_INIT_PNG;
    bool isRunning = true;
    screen = SDL_CreateWindow("Game1", 30, 0, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (screen == NULL) {

        err("Error in Creating Window");
    }


    if (!(IMG_Init(imgFlags) & imgFlags)) {
        err("Error in Image");
    }

    render = SDL_CreateRenderer(screen, -1, 0);
    tempSurface = IMG_Load("ship.png");
    texture = SDL_CreateTextureFromSurface(render, tempSurface);

    SDL_FreeSurface(tempSurface);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderPresent(render);
    bool boostGiven = false;
    while (isRunning) {
        Uint32 frameStart = SDL_GetTicks();
        float dt = get_delta_time();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
                break;
            }
            if (event.key.keysym.sym == SDLK_d) {
                angle += 3.5;
            }
            if (event.key.keysym.sym == SDLK_a) {
                angle -= 3.5;
            }
            if (event.key.keysym.sym == SDLK_w) {
                give_boost(200);
                // velocity.x += 100 * cos(angle);
                // velocity.y += 100 * sin(angle);
            }
        }
        update_spaceship(dt);
        SDL_RenderClear(render);
        SDL_RenderCopyEx(render, texture, &srcShip, &destShip, angle, nullptr, SDL_FLIP_NONE);
        SDL_RenderPresent(render);
        Uint32 frameEnd = SDL_GetTicks();
        if (frameEnd - frameStart < 16) {
            SDL_Delay(16 - (frameEnd - frameStart));
        }

    }
    std::cout << "QUITTING\n";
    SDL_Quit();
    return 0;

}
*/
