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

typedef struct partice {
    vector position;
    vector velocity;
    vector size;
    float mass;
} particle;

particle particles[PARTICLE_NUM];

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
        printf("cycle: %d", i);
        particles[i].position = (vector) {20, 20};
        particles[i].size = (vector) {20, 20};
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

    // draw rectangle
    SDL_Rect particle_rect = {
        particles[PARTICLE_NUM-1].position.x,
        particles[PARTICLE_NUM-1].position.y,
        particles[PARTICLE_NUM-1].size.x,
        particles[PARTICLE_NUM-1].size.y
    };
    

    // render
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &particle_rect);
    
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


