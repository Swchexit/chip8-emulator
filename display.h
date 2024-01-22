// Definition of chip8 display
// Created by Sheng Wei Chen on 2023/7/26.
//

#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H

#include "config.h"
#include <vector>

struct chip8_display {
    std::vector<std::vector<bool>> pixels; //[CHIP8_HEIGHT][CHIP8_WIDTH]
    chip8_display() : pixels(CHIP8_HEIGHT, std::vector<bool>(CHIP8_WIDTH)) {}

    static void validate_display(int x, int y);

    void clear();

    void set_XOR(int x, int y);

    bool is_set(int x, int y);

    bool draw_sprite(int x, int y, const uint8_t *sprite, int num);
};

#endif //CHIP8_DISPLAY_H
