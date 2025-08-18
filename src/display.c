// Mostly taken from here as it used a `renderer` rather than a `surface`
// https://thenumb.at/cpp-course/sdl2/05/05.html

#include "display.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

bool display_init(void) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Error initializing SDL: %s\n", SDL_GetError());
    return 1;
  }

  window =
      SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH * SCALE_FACTOR, WINDOW_HEIGHT * SCALE_FACTOR, SDL_WINDOW_SHOWN);

  if (!window) {
    printf("Error creating window: %s\n", SDL_GetError());
    return 1;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (!window) {
    printf("Error creating renderer: %s\n", SDL_GetError());
    return 1;
  }

  return true;
}

bool display_loop(void) {
  SDL_Event event;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  while (SDL_PollEvent(&event) != 0) {
    if (event.type == SDL_QUIT ||
        (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
      return false;
    }
  }

  SDL_RenderPresent(renderer);

  return true;
}

void display_cleanup(void) {
  if (renderer) {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
  }

  if (window) {
    SDL_DestroyWindow(window);
    window = NULL;
  }

  SDL_Quit();
}
