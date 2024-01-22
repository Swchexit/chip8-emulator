// Definition of chip8 keyboard
// Created by Sheng Wei Chen on 2023/7/26.

#ifndef CHIP8_KEYBOARD_H
#define CHIP8_KEYBOARD_H

#include "config.h"

struct chip8_keyboard {
    bool keyboard[CHIP8_NUM_KEYS];

    chip8_keyboard() : keyboard() {}

    static void validate_key(unsigned char key);

    static char key_mapping(const uint8_t *map, int key);

    void key_down(unsigned char key);

    void key_up(unsigned char key);

    bool key_is_down(unsigned char key);
};

#endif //CHIP8_KEYBOARD_H
