#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "chip8.h"

#define WINDOW_HEIGHT 32
#define WINDOW_WIDTH 64
#define SCALE_FACTOR 16

extern int sound_enabled;

bool display_init(void);
void display_render(Chip8 *chip8);
bool display_handle_events(Chip8 *chip8);
void display_cleanup(void);

#endif
