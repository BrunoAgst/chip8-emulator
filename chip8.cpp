//
// Created by Bruno Augusto on 04/09/24.
//

#include "chip8.h"
#include <iostream>
#include <iomanip>
#include <random>
#include "SDL.h"


void chip8::execute() {
    unsigned char m1 = memory[pc];
    unsigned char m2 = memory[pc + 1];
    unsigned short data = (m1 << 8) | m2;

    unsigned char opcode = (data >> 12) & 0xF;
    unsigned char d = (data >> 8) & 0xF;
    unsigned char dd = (data >> 4) & 0xF;
    unsigned char ddd = data & 0xF;

    selectInstruction(opcode, d, dd, ddd);
}

void chip8::selectInstruction(unsigned char opcode, unsigned char d, unsigned char dd, unsigned char ddd) {
    switch (opcode) {
        case 0x00:
            switch (((dd << 4) | ddd)) {
                case 0xE0:
                    std::memset(chip8ScreenBuffer, 0, sizeof(chip8ScreenBuffer));
                    drawFlag = true;
                    pc += 2;
                    break;
                case 0xEE:
                    sp--;
                    pc = stack[sp];
                    stack[sp] = 0x00;
                    pc += 2;
                    break;
                default:
                    std::cerr << "Instruction SYS invalid" << std::endl;
            }
            break;
        case 0x01:
            pc = ((d << 8) | (dd << 4) | ddd);
            break;
        case 0x02:
            stack[sp] = pc;
            sp++;
            pc = ((d << 8) | (dd << 4) | ddd);
            break;
        case 0x03:
            if(v[d] == ((dd << 4) | ddd)){
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x04:
            if(v[d] != ((dd << 4) | ddd)) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x05:
            if(v[d] == v[dd]){
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x06:
            v[d] = ((dd << 4) | ddd);
            pc += 2;
            break;
        case 0x07:
            v[d] += ((dd << 4) | ddd);
            pc += 2;
            break;
        case 0x08:
            eightInstructions(d, dd, ddd);
            pc += 2;
            break;
        case 0x09:
            if(v[d] != v[dd]){
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0x0A:
            i =  (d << 8) | (dd << 4) | ddd;
            pc += 2;
            break;
        case 0x0B:
            pc = ((d << 8) | (dd << 4) | ddd) + v[0x00];
            break;
        case 0x0C:
            cOperation(d, ((dd << 4) | ddd));
            pc += 2;
            break;
        case 0x0D:
            writeSprite(v[d], v[dd], ddd);
            pc += 2;
            break;
        case 0x0E:
            kInstructions(d, ((dd << 4) | ddd));
            break;
        case 0x0F:
            fInstructions(d, ((dd << 4) | ddd));
            break;
        default:
            std::cerr << "Invalid Instruction!!!" << std::endl;
            break;
    }
}

void chip8::eightInstructions(unsigned char vx, unsigned char vy, unsigned char opcode) {
    switch (opcode) {
        case 0x00:
            v[vx] = v[vy];
            break;
        case 0x01:
            v[vx] |= v[vy];
            break;
        case 0x02:
            v[vx] &= v[vy];
            break;
        case 0x03:
            v[vx] ^= v[vy];
            break;
        case 0x04:
            sum(vx, vy);
            break;
        case 0x05:
            sub(vx, vy);
            break;
        case 0x06:
            bitShiftRight(vx);
            break;
        case 0x07:
            subShift(vx, vy);
            break;
        case 0x0E:
            bitShiftLeft(vx);
            break;
        default:
            std::cerr << "Invalid ArithmeticOperation!!!" << std::endl;
            break;
    }
}

void chip8::sum(unsigned char vx, unsigned char vy) {
    unsigned short data = v[vx] + v[vy];
    if(data > 0xFF){
        v[0x0F] = 1;
    } else {
        v[0x0F] = 0;
    }
    v[vx] = data;
}

void chip8::sub(unsigned char vx, unsigned char vy) {
    unsigned short data = v[vx] - v[vy];
    if(data > 0xFF){
        v[0x0F] = 1;
    } else {
        v[0x0F] = 0;
    }
    v[vx] = data;
}

void chip8::cOperation(unsigned char r, unsigned char data) {
    v[r] = data & randomNumber();
}

unsigned char chip8::randomNumber() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 255);
    return distrib(gen);
}

void chip8::fInstructions(unsigned char x, unsigned char opcode) {
    switch (opcode) {
        case 0x07:
            v[x] = delay;
            pc += 2;
            break;
        case 0x0A:
            waitForPress(x);
            break;
        case 0x15:
            delay = v[x];
            pc += 2;
            break;
        case 0x18:
            sound = v[x];
            pc += 2;
            break;
        case 0x1E:
            i += v[x];
            pc += 2;
            break;
        case 0x29:
            i = v[x] * 5;
            pc += 2;
            break;
        case 0x33:
            fInstruction33(x);
            pc += 2;
            break;
        case 0x55:
            fInstruction55(x);
            pc += 2;
            break;
        case 0x65:
            fInstruction65(x);
            pc += 2;
            break;
        default:
            std::cerr << "Invalid Others Instructions!!!" << std::endl;
            break;
    }
}

void chip8::writeSprite(unsigned char x, unsigned char y, int height) {
    v[0xF] = 0x00;
    for(int row = 0; row < height; row++){
        unsigned char pixel = memory[i + row];
        for (int col = 0; col < 8; col++) {
            unsigned char bit = (pixel >> (7 - col)) & 1;
            if(bit == 0x01){
                unsigned char x_coord = (x + col) % 64;
                unsigned char y_coord = (y + row) % 32;

                if(chip8ScreenBuffer[x_coord][y_coord] == 1){
                    v[0xF] = 1;
                }
                chip8ScreenBuffer[x_coord][y_coord] ^= bit;
            }
        }
    }

    drawFlag = true;
}

void chip8::loadROM(const std::vector<unsigned char>& rom) {
    int x = pc;

    for(int j = 0; j < rom.size(); j++){
        memory[x + j] = rom[j];
    }
}

void chip8::loadFont() {
    unsigned char chip8Font[80] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0,
            0x20, 0x60, 0x20, 0x20, 0x70,
            0xF0, 0x10, 0xF0, 0x80, 0xF0,
            0xF0, 0x10, 0xF0, 0x10, 0xF0,
            0x90, 0x90, 0xF0, 0x10, 0x10,
            0xF0, 0x80, 0xF0, 0x10, 0xF0,
            0xF0, 0x80, 0xF0, 0x90, 0xF0,
            0xF0, 0x10, 0x20, 0x40, 0x40,
            0xF0, 0x90, 0xF0, 0x90, 0xF0,
            0xF0, 0x90, 0xF0, 0x10, 0xF0,
            0xF0, 0x90, 0xF0, 0x90, 0x90,
            0xE0, 0x90, 0xE0, 0x90, 0xE0,
            0xF0, 0x80, 0x80, 0x80, 0xF0,
            0xE0, 0x90, 0x90, 0x90, 0xE0,
            0xF0, 0x80, 0xF0, 0x80, 0xF0,
            0xF0, 0x80, 0xF0, 0x80, 0x80
    };

    int x = 0x50;

    for(int j = 0; j < 0x80; j++){
        memory[x + j] = chip8Font[j];
    }
}

void chip8::initCPU(const std::vector<unsigned char> &rom) {
    loadFont();
    loadROM(rom);
    initKeyboard();
}

void chip8::bitShiftRight(unsigned char vx) {
    unsigned char lsb = v[vx] % 2;
    v[0xF] = lsb;
    v[vx] >>= 1;
}

void chip8::bitShiftLeft(unsigned char vx) {
    unsigned char lsb = v[vx] % 2;
    v[0xF] = lsb;
    v[vx] <<= 1;
}

void chip8::subShift(unsigned char vx, unsigned char vy) {
    if(v[vy] >= v[vx]){
        v[0xF] = 1;
    } else {
        v[0xF] = 0;
    }

    v[vx] = v[vy] - v[vx];
}

void chip8::fInstruction65(unsigned char vx) {
    for(int reg = 0; reg <= vx; reg++){
        v[reg] = memory[i + reg];
    }
}

void chip8::fInstruction55(unsigned char vx) {
    for(int reg = 0; reg <= vx; reg++){
        memory[i + reg] = v[reg];
    }
}

void chip8::fInstruction33(unsigned char vx) {
    memory[i] = v[vx] / 100;
    memory[i + 1] = (v[vx] / 10) % 10;
    memory[i + 2] = v[vx] % 10;
}

void chip8::initKeyboard() {
    keyMapping[0] = SDL_SCANCODE_1;
    keyMapping[1] = SDL_SCANCODE_2;
    keyMapping[2] = SDL_SCANCODE_3;
    keyMapping[3] = SDL_SCANCODE_4;
    keyMapping[4] = SDL_SCANCODE_Q;
    keyMapping[5] = SDL_SCANCODE_W;
    keyMapping[6] = SDL_SCANCODE_E;
    keyMapping[7] = SDL_SCANCODE_R;
    keyMapping[8] = SDL_SCANCODE_A;
    keyMapping[9] = SDL_SCANCODE_S;
    keyMapping[10] = SDL_SCANCODE_D;
    keyMapping[11] = SDL_SCANCODE_F;
    keyMapping[12] = SDL_SCANCODE_Z;
    keyMapping[13] = SDL_SCANCODE_X;
    keyMapping[14] = SDL_SCANCODE_C;
    keyMapping[15] = SDL_SCANCODE_V;
}

void chip8::waitForPress(unsigned char vx) {
    std::cout << "Press Key!!!" << std::endl;
    bool keyP = false;
    while(!keyP){
        SDL_PumpEvents();
        const unsigned char* state = SDL_GetKeyboardState(NULL);
        for(int j = 0; j < 16; j++){
            if(state[keyMapping[j]]){
                v[vx] = j;
                keyP = true;
                break;
            }
        }
    }
}

void chip8::kInstructions(unsigned char vx, unsigned char opcode) {
    switch (opcode) {
        case 0x9E:
            if(keyState[v[vx]]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        case 0xA1:
            if(!keyState[v[vx]]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        default:
            std::cerr << "Invalid key instruction!!!" << std::endl;
    }
}
