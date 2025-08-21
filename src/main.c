#include "display.h"
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {

  srand(time(NULL));
  // Just for testing
  // for (int i = 0; i < argc; ++i) {
  //   printf("%s \n", argv[i]);
  // }

  // If no rom is supplied
  if (argc != 2) {
    printf("Wrong number of args\n");
    return 1;
  }

  if (!display_init()) {
    return 1;
  }

  bool running = true;

  Chip8 chip8;

  initialise(&chip8);
  load_rom(&chip8, argv[1]);

  while (running) {
    running = display_handle_events();
    if (execute_instruction(&chip8) < 0) {
      running = false;
    }
    display_render(&chip8);

    SDL_Delay(16);
  }

  display_cleanup();

  return 0;
}
