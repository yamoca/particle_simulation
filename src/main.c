#include <stdio.h>
#include <SDL2/SDL.h>

#define FALSE 0
#define TRUE 1

#define WIDTH 800
#define HEIGHT 800

#define PARTICLE_NUM 1 
#define TARGET_FRAME_TIME 60

int game_is_running = FALSE;
int last_frame_time;
float delta_time;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// makes a vector struct so we can easily reference points in 2d space
typedef struct vector {
    float x;
    float y;
} vector;

typedef struct particle {
    vector position;
    vector velocity;
    vector size;
    float mass;
} Particle;

Particle particles[PARTICLE_NUM];

// take a particle and return its force as a vector
// force = mass * acceleration (the acceleration is just earth's gravitational acceleration) 
// not negative because even though particles are moving downwards, sdl y coordinates increase going down
vector CalculateForce(Particle* particle) {
    return (vector) {0, particle->mass * 9.81};
}

int initiliaze_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "error initilaizing SDL. \n");
        return FALSE;
    }
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_BORDERLESS
    );
    if(!window) {
        fprintf(stderr, "error creating sdl window. \n");
        return FALSE;
    }
    renderer = SDL_CreateRenderer(window, -1, 0); 
    if(!renderer) {
        fprintf(stderr, "error creating sdl renderer. \n");
    }
   
    // everything worked
    return TRUE;
}

void process_input() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type) {
        // close window via x in top right
        case SDL_QUIT:
            game_is_running = FALSE;
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE) {
                game_is_running = FALSE;
                break;
            }
    }
}

void setup() {
    for(int i = 0; i < PARTICLE_NUM; i++) {
        Particle* particle = &particles[i];

        particle->position = (vector) {20 + i * 20, 20 + i * 20};
        particle->size = (vector) {20, 20};
        particle->mass = 1;
    }
}

void update() {
    // timestep logic
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + TARGET_FRAME_TIME)); // sleep until propper time has passed
    delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    last_frame_time = SDL_GetTicks();
}

void render() {
    // draw a black background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);


    // render
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    for(int i = 0; i < PARTICLE_NUM; i++) { 
        // create a pointer of type Particle (struct) called particle and assign it the memory address of particles[i]
        // we need a pointer so that variable is continuos 
        Particle* particle = &particles[i];

        vector force = CalculateForce(particle);
        // rearrange f = ma by dividing by m so: f/m = a
        vector acceleration = (vector) {force.x / particle->mass, force.y / particle->mass};

        particle->velocity.x += acceleration.x;
        particle->velocity.y += acceleration.y;

        particle->position.x += particle->velocity.x * delta_time;
        particle->position.y += particle->velocity.y * delta_time;
        
        // draw rectangle
        SDL_Rect particle_rect = {
                particle->position.x,   // -> is the same as . (eg particle.position) but for pointers 
                particle->position.y,
                particle->size.x,
                particle->size.y
        };

        printf("%f\n", particle->position.y);
        SDL_RenderFillRect(renderer, &particle_rect);
    };
    
    // must swap buffer as sdl uses double buffering
    SDL_RenderPresent(renderer);
}

void deconstruct() {
    // deconstruct in reverse order of initliaze
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char *argv[]) {
    game_is_running = initiliaze_window(); 
    setup();
    while(game_is_running) {
        process_input();
        update();
        render();
    }

    deconstruct();
    return 0;
}


