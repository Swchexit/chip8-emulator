// Definition of chip8 memory
// Created by Sheng Wei Chen on 2023/7/25.
//

#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include "config.h"
#include <vector>

struct chip8_memory {
    std::vector<uint8_t> memory;

    chip8_memory() : memory(CHIP8_MEM_SIZE) {}

    static void validate_memory_access(int index);

    void set(int index, unsigned char val);

    unsigned char get(int index);

    unsigned short get_short(int index);
};

#endif //CHIP8_MEMORY_H
