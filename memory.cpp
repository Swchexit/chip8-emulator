// Implementation of memory related functions
// Created by Sheng Wei Chen on 2023/7/26.

#include "memory.h"
#include "config.h"
#include <cassert>

void chip8_memory::validate_memory_access(int index) {
    assert(0 <= index && index < CHIP8_MEM_SIZE);
}

void chip8_memory::set(int index, unsigned char val) {
    validate_memory_access(index);
    memory[index] = val;
}

unsigned char chip8_memory::get(int index) {
    validate_memory_access(index);
    return memory[index];
}

unsigned short chip8_memory::get_short(int index) {
    validate_memory_access(index);
    validate_memory_access(index + 1);
    return (static_cast<unsigned short>(memory[index]) << 8 | memory[index + 1]);
}
