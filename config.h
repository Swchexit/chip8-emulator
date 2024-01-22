// configuration of the emulator
// Created by Sheng Wei Chen on 2023/7/25.
//

#ifndef CHIP8_CONFIG_H
#define CHIP8_CONFIG_H

#include <SDL2/SDL.h>

#define CHIP8_EMULATOR_TITLE "Chip8 Emulator X"
#define CHIP8_MEM_SIZE 4096
#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32
#define CHIP8_DISPLAY_SCALE 20
#define CHIP8_NUM_REGS 16
#define CHIP8_NUM_STACK 16
#define CHIP8_NUM_KEYS 16
#define CHIP8_CHARSET_LOAD_ADDR 0x00
#define CHIP8_PROGRAM_LOAD_ADDR 0x200
#define CHIP8_DEFAULT_SPRITE_HEIGHT 5

const uint8_t key_mapping[CHIP8_NUM_KEYS] = {
        SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5,
        SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b,
        SDLK_c, SDLK_d, SDLK_e, SDLK_f
};


#endif //CHIP8_CONFIG_H
