// Definition of CHIP8 emulators goes here
// Created by Sheng Wei Chen on 2023/7/25.

#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H

#include "config.h"
#include "memory.h"
#include "registers.h"
#include "stack.h"
#include "keyboard.h"
#include "display.h"
#include <cstring>
#include <random>

struct chip8 {
    struct chip8_memory memory;
    struct chip8_registers regs;
    struct chip8_stack stack;
    struct chip8_keyboard keyboard;
    struct chip8_display display;

    std::random_device rd;
    std::mt19937 e{rd()};
    std::uniform_int_distribution<int> dist{0, 255};

    chip8();

    void load_program(const std::vector<uint8_t> &buf);

    int exec(unsigned short opcode);

    int exec_extended(unsigned short opcode);

    int exec_extended_8(unsigned short opcode);

    int exec_extended_F(unsigned short opcode);

    static unsigned char wait_for_key_press();
};

const uint8_t chip8_default_character_set[] = {
        0xf0, 0x90, 0x90, 0x90, 0xf0,
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xf0, 0x10, 0xf0, 0x80, 0xf0,
        0xf0, 0x10, 0xf0, 0x10, 0xf0,
        0x90, 0x90, 0xf0, 0x10, 0x10,
        0xf0, 0x80, 0xf0, 0x10, 0xf0,
        0xf0, 0x80, 0xf0, 0x90, 0xf0,
        0xf0, 0x10, 0x20, 0x40, 0x40,
        0xf0, 0x90, 0xf0, 0x90, 0xf0,
        0xf0, 0x90, 0xf0, 0x10, 0xf0,
        0xf0, 0x90, 0xf0, 0x90, 0x90,
        0xe0, 0x90, 0xe0, 0x90, 0xe0,
        0xf0, 0x80, 0x80, 0x80, 0xf0,
        0xe0, 0x90, 0x90, 0x90, 0xe0,
        0xf0, 0x80, 0xf0, 0x80, 0xf0,
        0xf0, 0x80, 0xf0, 0x80, 0x80
};
#endif //CHIP8_CHIP8_H
