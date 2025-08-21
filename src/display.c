// Mostly taken from here as it used a `renderer` rather than a `surface`
// https://thenumb.at/cpp-course/sdl2/05/05.html

#include "display.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <stdbool.h>
#include <stdio.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

bool display_init(void) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Error initializing SDL: %s\n", SDL_GetError());
    return false;
  }

  window =
      SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH * SCALE_FACTOR,
                       WINDOW_HEIGHT * SCALE_FACTOR, SDL_WINDOW_SHOWN);
  if (!window) {
    printf("Error creating window: %s\n", SDL_GetError());
    SDL_Quit();
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    printf("Error creating renderer: %s\n", SDL_GetError());
    return false;
  }

  // 64 x 32 is the "base" width and height of the Chip8 display which we will
  // scale up
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                              SDL_TEXTUREACCESS_STREAMING, 64, 32);
  if (!texture) {
    printf("Error creating texture: %s\n", SDL_GetError());
    return false;
  }

  return true;
}

bool display_handle_events(void) {
  SDL_Event event;

  while (SDL_PollEvent(&event) != 0) {
    switch (event.type) {
    case SDL_QUIT:
      return false;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        return false;
      }
    }

  }

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

// https://www.reddit.com/r/EmuDev/comments/4pclt1/chip8_and_sdl/
void display_render(Chip8 *chip8) {
  // We only want to draw when the draw flag is set
  if (!chip8->draw_flag)
    return;

  static uint32_t pixels[DISPLAY_WIDTH * DISPLAY_HEIGHT];

  for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
    if (chip8->display[i]) {
      pixels[i] = 0xFFFFFFFF; // WHITE
    } else {
      pixels[i] = 0xFF000000; // BLACK
    }
  }

  // Pitch size what is it?????
  SDL_UpdateTexture(texture, NULL, pixels, DISPLAY_WIDTH * sizeof(uint32_t));

  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  chip8->draw_flag = 0;
}
