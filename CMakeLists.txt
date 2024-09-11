cmake_minimum_required(VERSION 3.28)
project(chip_8)

set(CMAKE_CXX_STANDARD 17)
set(SDL2_DIR "/opt/homebrew/Cellar/sdl2/2.30.7/lib/cmake/SDL2")

add_executable(chip_8 main.cpp
        chip8.h
        chip8.cpp
        display.h
        display.cpp
        file.h
        file.cpp
)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(chip_8 ${SDL2_LIBRARIES})
