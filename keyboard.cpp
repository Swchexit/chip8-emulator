// Implementations of chip8 keyboard
// Created by Sheng Wei Chen on 2023/7/26.

#include "keyboard.h"
#include <cassert>

void chip8_keyboard::validate_key(unsigned char key) {
    assert(0 <= key && key < CHIP8_NUM_KEYS);
}

char chip8_keyboard::key_mapping(const uint8_t *map, int key) {
    for (char i = 0; i < CHIP8_NUM_KEYS; i++) {
        if (map[i] == key)
            return i;
    }
    return -1;
}

void chip8_keyboard::key_down(unsigned char key) {
    validate_key(key);
    keyboard[key] = true;
}

void chip8_keyboard::key_up(unsigned char key) {
    validate_key(key);
    keyboard[key] = false;
}

bool chip8_keyboard::key_is_down(unsigned char key) {
    validate_key(key);
    return keyboard[key];
}