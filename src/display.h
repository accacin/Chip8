#include <SDL2/SDL.h>
#include <stdbool.h>

#define WINDOW_HEIGHT 32
#define WINDOW_WIDTH 64
#define SCALE_FACTOR 16

bool display_init(void);
bool display_loop(void);
void display_cleanup(void);
// void display_cleanup(void);
// SDL_Renderer* display_get_renderer(void); // To access renderer from main
