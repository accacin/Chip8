CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LIBS = -lSDL2

chip8: src/main.c src/display.c src/chip8.c
	$(CC) $(CFLAGS) -o build/chip8 src/main.c src/display.c src/chip8.c $(LIBS)
	
clean: 
	rm -rf build/chip8

.PHONY: clean
