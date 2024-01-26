cmake_minimum_required(VERSION 3.24)
project(chip8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_SKIP_BUILD_RPATH OFF)
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
set(CMAKE_INSTALL_RPATH "/Library/Frameworks")
set(CMAKE_BUILD_RPATH "/Library/Frameworks")


find_package(SDL2 REQUIRED COMPONENTS SDL2)
add_executable(chip8 main.cpp chip8.h config.h memory.h memory.cpp registers.h stack.cpp stack.h keyboard.cpp keyboard.h display.cpp display.h chip8.cpp)
target_link_libraries(chip8 PRIVATE SDL2::SDL2)

add_custom_command(
        TARGET chip8 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/game_roms/INVADERS
        ${CMAKE_CURRENT_BINARY_DIR}/INVADERS)