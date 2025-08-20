#include "chip8.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static const uint8_t fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void initialise(Chip8 *chip8) {
  memset(chip8, 0, sizeof(Chip8));
  chip8->pc = PROGRAM_START;
  for (int i = 0; i < 80; i++) {
    chip8->memory[FONT_START + i] = fontset[i];
  }
}

void load_rom(Chip8 *chip8, const char *filename) {
  long rom_length;

  FILE *fptr = fopen(filename, "rb");
  if (fptr != NULL) {
    fseek(fptr, 0, SEEK_END);
    rom_length = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    if (rom_length > (4096 - PROGRAM_START)) {
      printf("Error: ROM too large for memory\n");
      fclose(fptr);
      return;
    }

    size_t bytes_read =
        fread(&chip8->memory[PROGRAM_START], 1, rom_length, fptr);

    if (bytes_read != (size_t)rom_length) {
      printf("Error: Bytes read don't match the size of the ROM\n");
      fclose(fptr);
      return;
    }
    fclose(fptr);
    printf("ROM loaded sucessfully: %ld bytes\n", rom_length);
  } else {
    printf("ERROR: Invalid ROM path: '%s'\n", filename);
  }
}

int execute_instruction(Chip8 *chip8) {
  // Todo how to get an instruction
  // how to erad it
  uint16_t opcode;
  opcode = (chip8->memory[chip8->pc] << 8) | chip8->memory[chip8->pc + 1];
  // printf("DEBUG: Executing opcode 0x%X at PC 0x%X\n", opcode, chip8->pc);

  // https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#instructions
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  uint8_t n = (opcode & 0x000F);
  uint8_t nn = (opcode & 0x00FF);
  uint16_t nnn = (opcode & 0x0FFF);

  // TODO for IBM
  // 00E0 - clear
  // 1NNN - jump
  // 6XNN - sex reg vx
  // 7XNN - add value to vx
  // ANNN - set index register I
  // DXYN - display/draw

  switch (opcode & 0xF000) {
  case 0x0000:
    switch (nn) {
    case 0xE0:
      memset(chip8->display, 0, sizeof(chip8->display));
      chip8->draw_flag = 1;
      chip8->pc += 2;
      break;
    case 0xEE:
      chip8->sp -= 1;
      chip8->pc = chip8->stack[chip8->sp];
      break;
    default:
    printf("Unimplemented opcode: 0x%X\n", opcode);
      return -1;
    }
    break;
  case 0x1000:
    chip8->pc = nnn;
    chip8->pc += 2;
    break;
  case 0x2000:
    // TODO: Handle stack overflow?
    chip8->stack[chip8->sp] = chip8->pc += 2;
    chip8->sp += 1;
    chip8->pc = nnn;
    break;
  case 0x3000:
    if (chip8->V[x] == nn)
      chip8->pc += 2;
    chip8->pc += 2;
    break;
  case 0x4000:
    if (chip8->V[x] != nn)
      chip8->pc += 2;
    chip8->pc += 2;
    break;
  case 0x5000:
    if (chip8->V[x] == chip8->V[y])
      chip8->pc += 2;
    chip8->pc += 2;
    break;
  case 0x6000:
    chip8->V[x] = nn;
    chip8->pc += 2;
    break;
  case 0x7000:
    chip8->V[x] += nn;
    chip8->pc += 2;
    break;
  case 0x8000:
    switch (n) {
    case 0x0:
      chip8->V[x] = chip8->V[y];
      chip8->pc += 2;
      break;
    case 0x5:
      chip8->V[0xF] = chip8->V[x] >= chip8->V[y] ? 1 : 0;
      chip8->V[x] = chip8->V[x] - chip8->V[y];
      chip8->pc += 2;
      break;
    case 0x7:
      chip8->V[0xF] = chip8->V[x] >= chip8->V[y] ? 1 : 0;
      chip8->V[x] = chip8->V[y] - chip8->V[x];
      chip8->pc += 2;
      break;
    default:
    printf("Unimplemented opcode: 0x%X\n", opcode);
      return -1;
    }
    break;
  case 0x9000:
    if (chip8->V[x] != chip8->V[y])
      chip8->pc += 2;
    chip8->pc += 2;
    break;
  case 0xA000:
    chip8->I = nnn;
    chip8->pc += 2;
    break;
    // case 0xB000:
    //   break;
    // case 0xC000:
    //   break;
  case 0xD000: {
    // The first thing to do is to get the X and Y coordinates from VX and VY.
    uint8_t x_coord = chip8->V[x];
    uint8_t y_coord = chip8->V[y];

    // Set the X coordinate to the value in VX modulo 64 (or, equivalently, VX &
    // 63, where & is the binary AND operation)
    int x_wrap = x_coord & 63;
    // Set the Y coordinate to the value in VX modulo 64 (or, equivalently, VX &
    // 63, where & is the binary AND operation)
    int y_wrap = y_coord & 31;
    // Set VF to 0
    chip8->V[0xF] = 0;

    for (int row = 0; row < n; row++) {
      // Stop if you reach the bottom edge of the screen
      if ((y_wrap + row) >= 32)
        break;
      // Get the Nth byte of sprite data, counting from the memory address in
      // the I register (I is not incremented)
      uint8_t sprite = chip8->memory[chip8->I + row];
      // For each of the 8 pixels/bits in this sprite row (from left to right,
      // ie. from most to least significant bit):
      for (int col = 0; col < 8; col++) {
        int screen_x = x_wrap + col;
        int screen_y = y_wrap + row;

        // If you reach the right edge of the screen, stop drawing this row
        if (screen_x >= 64)
          break;

        // Current pixel
        int sprite_pixel = (sprite >> (7 - col)) & 1;
        // Convert screen_y and screen_x into an index (1d array)
        int screen_index = screen_y * 64 + screen_x;

        // If the current pixel in the sprite row is on
        if (sprite_pixel) {
          int screen_pixel = chip8->display[screen_index];
          // and the pixel at coordinates X,Y on the screen is also on,
          if (screen_pixel) {
            // turn off the pixel
            chip8->display[screen_index] = 0;
            // and set VF to 1
            chip8->V[0xF] = 1;
          } else {
            // Or if the current pixel in the sprite row is on and the screen
            // pixel is not, draw the pixel at the X and Y coordinates
            chip8->display[screen_index] = 1;
          }
        }
      }
    }
    chip8->draw_flag = 1;
    chip8->pc += 2;
    break;

  } break;
    // case 0xE000:
    //   break;
  case 0xF000: // NOT SURE
    switch (nn) {
    case 0x29:
      chip8->I = chip8->memory[FONT_START + x];
      chip8->I = FONT_START + (chip8->V[x] * 5);
      chip8->pc += 2;
      break;
    default:
    printf("Unimplemented opcode: 0x%X\n", opcode);
      return -1;
    }
    break;
  default:
    printf("Unimplemented opcode: 0x%X\n", opcode);
    return -1;
  }
  return 1;
}
