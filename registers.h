// Definition of chip8 register file
// Created by Sheng Wei Chen on 2023/7/26.

#ifndef CHIP8_REGISTERS_H
#define CHIP8_REGISTERS_H

#include "config.h"

struct chip8_registers {
    unsigned char V[CHIP8_NUM_REGS];    // 16 8-bit general purpose registers 'V'
    unsigned short I;                   // 16-bit register 'I'
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short PC;
    unsigned char SP;

    chip8_registers() : V(), I(), delay_timer(), sound_timer(), PC(), SP() {}
};

#endif //CHIP8_REGISTERS_H
