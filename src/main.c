#include "chip8.h"
#include "display.h"
#include <SDL2/SDL_timer.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

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

  while (display_loop()) {
    SDL_Delay(10);
  }

  display_cleanup();

  return 0;
}
