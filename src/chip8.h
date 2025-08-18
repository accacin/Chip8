#include <stdint.h>

// https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/

#define FONT_START 0x050
#define FONT_END 0x09F
#define PROGRAM_START 0x200
#define PROGRAM_END 0xFFF
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

typedef struct {
  uint8_t memory[4096];                                // Memory
  uint8_t V[16];                                       // General purpose registers
  uint16_t I;                                          // Instruction counter
  uint16_t pc;                                         // Program counter
  uint16_t stack[16];                                  // 16 according to cowgod's docs
  uint8_t sp;                                          // Stack pointer
  uint8_t delay_timer;
  uint8_t sound_timer;                                 // Sound when > 0
  uint8_t keys[16];                                    // 4*4 grid of keys on a keyboard
  uint8_t display[DISPLAY_WIDTH*DISPLAY_HEIGHT];
  uint8_t draw_flag;                                   // Only draw when this is set via opcode
} Chip8;

// Function Declarations
void initialise(Chip8 *chip8);
void load_rom(Chip8 *chip8, const char *filename);
int execute_instruction(Chip8 *chip8);
