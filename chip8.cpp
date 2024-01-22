// Basic utility and Instructions set simulation of chip8
// Created by Sheng Wei Chen on 2023/7/26.


#include "chip8.h"
#include <iostream>
#include <cassert>
#include <algorithm>
#include <random>
#include <SDL2/SDL.h>

chip8::chip8() : memory(), regs(), stack(), keyboard(), display() {
    // Loading interpreter into memory
    std::copy(std::begin(chip8_default_character_set),
              std::end(chip8_default_character_set),
              memory.memory.begin() + CHIP8_CHARSET_LOAD_ADDR);
}

void chip8::load_program(const std::vector<uint8_t> &buf) {
    assert (buf.size() + CHIP8_PROGRAM_LOAD_ADDR <= CHIP8_MEM_SIZE);
    std::copy(buf.begin(), buf.end(), memory.memory.begin() + CHIP8_PROGRAM_LOAD_ADDR);
    regs.PC = CHIP8_PROGRAM_LOAD_ADDR;
}

int chip8::exec(unsigned short opcode) {
    switch (opcode) {
        case 0x00e0:
            display.clear();
            break;
        case 0x00ee:
            regs.PC = stack.pop(*this);
            break;
        default:
            if (exec_extended(opcode) < 0) {
                // ERROR!!! TERMINATE THE PROGRAM
                std::cout << opcode << ' ' << regs.PC << std::endl;
                std::cerr << "Operation not supported, aborting" << std::endl;
                return -1;
            }
    }
    return 0;
}

unsigned char chip8::wait_for_key_press() {
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type != SDL_KEYDOWN)
            continue;
        int key = event.key.keysym.sym;
        char virtual_key = chip8_keyboard::key_mapping(key_mapping, key);
        if (virtual_key != -1)
            return virtual_key;
    }
    return 0;
}

int chip8::exec_extended_8(unsigned short opcode) {
    unsigned short x = (opcode >> 8) & 0x000f;
    unsigned short y = (opcode >> 4) & 0x000f;
    unsigned short n = opcode & 0x000f;
    unsigned short tmp;
    switch (n) {
        case 0x00:
            regs.V[x] = regs.V[y];
            break;
        case 0x01:
            regs.V[x] |= regs.V[y];
            break;
        case 0x02:
            regs.V[x] &= regs.V[y];
            break;
        case 0x03:
            regs.V[x] ^= regs.V[y];
            break;
        case 0x04:
            tmp = regs.V[x] + regs.V[y];
            regs.V[0x0f] = false;
            if (tmp > 0xff)
                regs.V[0x0f] = true;
            regs.V[x] = tmp & 0xff;
            break;
        case 0x05:
            regs.V[0x0f] = false;
            if (regs.V[x] > regs.V[y])
                regs.V[0x0f] = true;
            regs.V[x] -= regs.V[y];
            break;
        case 0x06:
            regs.V[0x0f] = regs.V[x] & 0x01;
            // regs.V[x] >>= 1;
            regs.V[x] /= 2;
            break;
        case 0x07:
            regs.V[0x0f] = (regs.V[y] > regs.V[x]) ? 1 : 0;
            regs.V[x] = regs.V[y] - regs.V[x];
            break;
        case 0x0E:
            regs.V[0x0f] = (regs.V[x] & 0b10000000);
            // regs.V[x] <<= 1;
            regs.V[x] *= 2;
            break;
        default:
            return -1;
    }
    return 0;
}

int chip8::exec_extended_F(unsigned short opcode) {
    unsigned short x = (opcode >> 8) & 0x000f;
    unsigned short n = opcode & 0x00ff;
    unsigned short tmp;
    switch (n) {
        case 0x07:
            regs.V[x] = regs.delay_timer;
            break;
        case 0x0a:
            tmp = wait_for_key_press();
            regs.V[x] = tmp;
            break;
        case 0x15:
            regs.delay_timer = regs.V[x];
            break;
        case 0x18:
            regs.sound_timer = regs.V[x];
            break;
        case 0x1e:
            regs.I += regs.V[x];
            break;
        case 0x29:
            // FIXME: Add base of sprites here?
            regs.I = regs.V[x] * CHIP8_DEFAULT_SPRITE_HEIGHT;
            break;
        case 0x33:
            memory.set(regs.I, regs.V[x] / 100);
            memory.set(regs.I + 1, (regs.V[x] / 10) % 10);
            memory.set(regs.I + 2, regs.V[x] % 10);
            break;
        case 0x55: {
            for (int i = 0; i <= x; i++) {
                memory.set(regs.I + i, regs.V[i]);
            }
            break;
        }
        case 0x65: {
            for (int i = 0; i <= x; i++) {
                regs.V[i] = memory.get(regs.I + i);
            }
            break;
        }
        default:
            return -1;
    }
    return 0;
}

int chip8::exec_extended(unsigned short opcode) {
    unsigned short nnn = opcode & 0x0fff;
    unsigned short x = (opcode >> 8) & 0x000f;
    unsigned short y = (opcode >> 4) & 0x000f;
    unsigned short n = opcode & 0x000f;
    unsigned short kk = opcode & 0x00ff;
    unsigned short tmp;
    switch (opcode & 0xf000) {
        case 0x1000:    // JP addr: Jump to location nnn.
            regs.PC = nnn;
            break;
        case 0x2000:      // CALL addr: Call subroutine at nnn.
            stack.push(*this, regs.PC);
            regs.PC = nnn;
            break;
        case 0x3000:      // SE Vx, byte: Skip next instruction if Vx = kk.
            if (regs.V[x] == kk)
                regs.PC += 2;
            break;
        case 0x4000:
            if (regs.V[x] != kk)
                regs.PC += 2;
            break;
        case 0x5000:
            if (regs.V[x] == regs.V[y])
                regs.PC += 2;
            break;
        case 0x6000:      // LD Vx, byte: Set Vx = kk.
            regs.V[x] = kk;
            break;
        case 0x7000:      // ADD Vx, byte: Set Vx = Vx + kk.
            regs.V[x] += kk;
            break;
        case 0x8000:      // 8xy[1..7^E] are 8 different instructions.
            exec_extended_8(opcode);
            break;
        case 0x9000:
            if (regs.V[x] != regs.V[y])
                regs.PC += 2;
            break;
        case 0xA000:
            regs.I = nnn;
            break;
        case 0xB000:
            regs.PC = nnn + regs.V[0];
            break;
        case 0xC000:
            regs.V[x] = dist(e) & kk;
            break;
        case 0xD000:
            regs.V[0x0f] = 0;
            if (display.draw_sprite(regs.V[x], regs.V[y], &memory.memory[regs.I], n))
                regs.V[0x0f] = 1;
            break;
        case 0xE000:
            tmp = (opcode & 0x00ff);
            if (tmp == 0x009e) {
                if (keyboard.key_is_down(regs.V[x]))
                    regs.PC += 2;
            } else if (tmp == 0x00a1) {
                if (!keyboard.key_is_down(regs.V[x]))
                    regs.PC += 2;
            } else
                return -1;
            break;
        case 0xF000:
            exec_extended_F(opcode);
            break;
        default:
            return -1;
    }
    return 0;
}