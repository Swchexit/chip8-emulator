#include <iostream>
#include <SDL2/SDL.h>
#include <cstdio>
#include <thread>
#include <chrono>
#include "chip8.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "You must provide the file to run." << std::endl;
        return -1;
    }
    const char *filename = argv[1];

    auto fp = std::fopen(filename, "rb");
    if (!fp) {
        std::cout << "Error opening file" << filename << ", aborting" << std::endl;
        return -1;
    }

    std::fseek(fp, 0, SEEK_END);
    const std::size_t filesize = std::ftell(fp);
    std::vector<std::uint8_t> buffer(filesize);
    std::fseek(fp, 0, SEEK_SET);
    if (std::fread(buffer.data(), sizeof(std::uint8_t), buffer.size(), fp) < filesize) {
        std::cout << "Failed to read file " << filename << std::endl;
        std::fclose(fp);
        return -1;
    }
    std::fclose(fp);

    struct chip8 chip8;
    chip8.load_program(buffer);
    std::cout << "File " << filename << " with " << filesize << " bytes loaded, starting chip8" << std::endl;
    std::cout << "-------- Chip8 LAUNCH!!!!!!!!!! --------" << std::endl;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
            CHIP8_EMULATOR_TITLE,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            CHIP8_WIDTH * CHIP8_DISPLAY_SCALE, CHIP8_HEIGHT * CHIP8_DISPLAY_SCALE, SDL_WINDOW_SHOWN
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
    // Main loop
    while (true) {
        auto start = std::chrono::system_clock::now();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    goto out;
                case SDL_KEYDOWN: {
                    auto key = event.key.keysym.sym;
                    char virtual_key = chip8_keyboard::key_mapping(key_mapping, key);
                    if (virtual_key != -1)
                        chip8.keyboard.key_down(virtual_key);
                    break;
                }
                case SDL_KEYUP: {
                    auto key = event.key.keysym.sym;
                    char virtual_key = chip8_keyboard::key_mapping(key_mapping, key);
                    if (virtual_key != -1)
                        chip8.keyboard.key_up(virtual_key);
                    break;
                }
                default:
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        for (int x = 0; x < CHIP8_WIDTH; x++) {
            for (int y = 0; y < CHIP8_HEIGHT; y++) {
                if (chip8.display.is_set(x, y)) {
                    SDL_Rect r;
                    r.x = x * CHIP8_DISPLAY_SCALE;
                    r.y = y * CHIP8_DISPLAY_SCALE;
                    r.w = 1 * CHIP8_DISPLAY_SCALE;
                    r.h = 1 * CHIP8_DISPLAY_SCALE;
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }
        SDL_RenderPresent(renderer);
        if (chip8.regs.delay_timer > 0) {
            std::this_thread::sleep_for(std::chrono::microseconds (100));
            chip8.regs.delay_timer--;
        }
        if (chip8.regs.sound_timer > 0) {
            std::cout << "Some sound being played" << std::endl;
            chip8.regs.sound_timer -= 1;
        }

        unsigned short opcode = chip8.memory.get_short(chip8.regs.PC);
        chip8.regs.PC += 2;
        if (chip8.exec(opcode) < 0)
            return -1;

        // Some computation here
        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end-start;
        // std::cerr << elapsed_seconds.count() << std::endl;
    }

out:
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
