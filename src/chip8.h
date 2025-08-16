#include <stdint.h>

// https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/

#define FONT_START 0x050
#define FONT_END 0x09F
#define PROGRAM_START 0x200
#define PROGRAM_END 0xFFF

// Where to go?
// uint16_t opcode;

typedef struct {
  uint8_t memory[4096];                                // Memory
  uint8_t V[16];                                       // General purpose registers
  uint16_t I;                                          // Instruction counter
  uint16_t pc;                                         // Program counter
} Chip8;
