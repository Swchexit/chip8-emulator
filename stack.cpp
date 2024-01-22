// Implementations of chip8 stack
// Created by Sheng Wei Chen on 2023/7/26.
//

#include "stack.h"
#include "chip8.h"
#include <cassert>

void chip8_stack::validate_stack_access(struct chip8 &chip8) {
    assert(chip8.regs.SP < CHIP8_NUM_STACK);
}

void chip8_stack::push(chip8 &chip8, unsigned short val) {
    validate_stack_access(chip8);
    stack[chip8.regs.SP++] = val;
}

unsigned short chip8_stack::pop(chip8 &chip8) {
    chip8.regs.SP--;
    validate_stack_access(chip8);
    return stack[chip8.regs.SP];
}