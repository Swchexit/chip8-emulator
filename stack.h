// Definition of chip8 stack
// Created by Sheng Wei Chen on 2023/7/26.

#ifndef CHIP8_STACK_H
#define CHIP8_STACK_H

#include "config.h"

struct chip8;       // Forward declaration for struct chip8
struct chip8_stack {
    unsigned short stack[CHIP8_NUM_STACK];

    chip8_stack() : stack() {}

    static void validate_stack_access(struct chip8 &chip8);

    void push(chip8 &chip8, unsigned short val);

    unsigned short pop(chip8 &chip8);
};


#endif //CHIP8_STACK_H
