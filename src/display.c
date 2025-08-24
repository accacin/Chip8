// Mostly taken from here as it used a `renderer` rather than a `surface`
// https://thenumb.at/cpp-course/sdl2/05/05.html

#include "display.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_stdinc.h>
#include <stdbool.h>
#include <stdio.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

const int SAMPLE_RATE = 22050;
const int BUFFER_SIZE = 4096;

static float phase = 0.0f;
static float frequency = 440.0f; // Hz

int sound_enabled = 1;

void callback(void *userdata, Uint8 *stream, int len);

SDL_AudioSpec spec = {
    .format = AUDIO_F32,
    .channels = 1,
    .freq = SAMPLE_RATE,
    .samples = BUFFER_SIZE,
    .callback = callback,
};

bool display_init(void) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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

  if (SDL_OpenAudio(&spec, NULL) < 0) {
    printf("Error creating texture: %s\n", SDL_GetError());
    SDL_Quit();
    return false;
  }

  SDL_PauseAudio(0);

  return true;
}

bool display_handle_events(Chip8 *chip8) {
  SDL_Event event;

  while (SDL_PollEvent(&event) != 0) {
    switch (event.type) {
    case SDL_QUIT:
      return false;

    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        return false;
      case SDLK_1:
        chip8->keys[0x1] = true;
        break;
      case SDLK_2:
        chip8->keys[0x2] = true;
        break;
      case SDLK_3:
        chip8->keys[0x3] = true;
        break;
      case SDLK_4:
        chip8->keys[0xC] = true;
        break;
      case SDLK_q:
        chip8->keys[0x4] = true;
        break;
      case SDLK_w:
        chip8->keys[0x5] = true;
        break;
      case SDLK_e:
        chip8->keys[0x6] = true;
        break;
      case SDLK_r:
        chip8->keys[0xD] = true;
        break;
      case SDLK_a:
        chip8->keys[0x7] = true;
        break;
      case SDLK_s:
        chip8->keys[0x8] = true;
        break;
      case SDLK_d:
        chip8->keys[0x9] = true;
        break;
      case SDLK_f:
        chip8->keys[0xE] = true;
        break;
      case SDLK_z:
        chip8->keys[0xA] = true;
        break;
      case SDLK_x:
        chip8->keys[0x0] = true;
        break;
      case SDLK_c:
        chip8->keys[0xB] = true;
        break;
      case SDLK_v:
        chip8->keys[0xF] = true;
        break;
      }
      break;

    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
      case SDLK_1:
        chip8->keys[0x1] = false;
        break;
      case SDLK_2:
        chip8->keys[0x2] = false;
        break;
      case SDLK_3:
        chip8->keys[0x3] = false;
        break;
      case SDLK_4:
        chip8->keys[0xC] = false;
        break;
      case SDLK_q:
        chip8->keys[0x4] = false;
        break;
      case SDLK_w:
        chip8->keys[0x5] = false;
        break;
      case SDLK_e:
        chip8->keys[0x6] = false;
        break;
      case SDLK_r:
        chip8->keys[0xD] = false;
        break;
      case SDLK_a:
        chip8->keys[0x7] = false;
        break;
      case SDLK_s:
        chip8->keys[0x8] = false;
        break;
      case SDLK_d:
        chip8->keys[0x9] = false;
        break;
      case SDLK_f:
        chip8->keys[0xE] = false;
        break;
      case SDLK_z:
        chip8->keys[0xA] = false;
        break;
      case SDLK_x:
        chip8->keys[0x0] = false;
        break;
      case SDLK_c:
        chip8->keys[0xB] = false;
        break;
      case SDLK_v:
        chip8->keys[0xF] = false;
        break;
      }
      break;
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

// Interesting article on sound generation
// https://gist.github.com/jacobsebek/10867cb10cdfccf1d6cfdd24fa23ee96
void callback(void *userdata, Uint8 *stream, int len) {
  float *samples = (float *)stream;
  int sample_count = len / sizeof(float);

  for (int i = 0; i < sample_count; i++) {
    if (sound_enabled) {
      samples[i] = (phase < 0.5f) ? 0.1f : -0.1f;

      phase += frequency / SAMPLE_RATE;
      if (phase >= 1.0f)
        phase -= 1.0f;
    } else {
      samples[i] = 0.0f;
    }
  }
}
