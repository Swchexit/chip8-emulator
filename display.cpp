// Implementation of chip8 display
// Created by Sheng Wei Chen on 2023/7/26.
//

#include "display.h"
#include <cassert>

void chip8_display::validate_display(int x, int y) {
    assert((0 <= x && x < CHIP8_WIDTH) && (0 <= y && y < CHIP8_HEIGHT));
}

void chip8_display::clear() {
    for (auto &i: pixels)
        std::fill(i.begin(), i.end(), false);
}

void chip8_display::set_XOR(int x, int y) {
    validate_display(x, y);
    pixels[y][x] = !pixels[y][x];
}

bool chip8_display::is_set(int x, int y) {
    validate_display(x, y);
    return pixels[y][x];
}

bool chip8_display::draw_sprite(int x, int y, const uint8_t *sprite, int num) {
    bool pixel_collision = false;
    for (int ly = 0; ly < num; ly++) {
        unsigned char c = sprite[ly];
        for (int lx = 0; lx < 8; lx++) {
            if ((c & (0b10000000 >> lx)) == 0)
                continue;
            if (is_set((lx + x) % CHIP8_WIDTH, (ly + y) % CHIP8_HEIGHT))
                pixel_collision = true;
            set_XOR((lx + x) % CHIP8_WIDTH, (ly + y) % CHIP8_HEIGHT);
        }
    }
    return pixel_collision;
}
