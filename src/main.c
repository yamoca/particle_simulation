#include <stdio.h>
#include <SDL2/SDL.h>
#include "constants.h"

int game_is_running = FALSE;
int last_frame_time;
float delta_time;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

struct ball {
    float x;
    float y;
    float width;
    float height;
} ball;

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
    ball.x = 20;
    ball.y = 20;
    ball.width = 15;
    ball.height = 15;
}

void update() {
    // timestep logic
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + TARGET_FRAME_TIME)); // sleep until propper time has passed

    delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    last_frame_time = SDL_GetTicks();

    ball.x += 50 * delta_time;
    ball.y += 50 * delta_time;
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // draw rectangle
    SDL_Rect ball_rect = {
        (int)ball.x,
        (int)ball.y,
        (int)ball.width,
        (int)ball.height
    };
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);
    
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

