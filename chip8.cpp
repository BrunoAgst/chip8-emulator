//
// Created by Bruno Augusto on 04/09/24.
//

#include "chip8.h"
#include <iostream>
#include <iomanip>
#include <random>


void chip8::execute() {
    unsigned short data = (memory[pc] << 8) | memory[pc + 1];

    unsigned char opcode = (data >> 12) & 0xF;
    unsigned char d = (data >> 8) & 0xF;
    unsigned char dd = (data >> 4) & 0xF;
    unsigned char ddd = data & 0xF;

    selectInstruction(opcode, d, dd, ddd);
}

void chip8::selectInstruction(unsigned char opcode, unsigned char d, unsigned int dd, unsigned char ddd) {
    switch (opcode) {
        case 0x00:
            if(dd == 0x0E && ddd == 0x0E) {
                sp--;
                pc = stack[sp];
            } else {
                std::memset(chip8ScreenBuffer, 0, sizeof(chip8ScreenBuffer));
                drawFlag = true;
                pc += 2;
            }
            break;
        case 0x01:
            pc = (d << 8) | (dd << 4) | ddd;
            break;
        case 0x02:
            stack[sp] = pc;
            sp++;
            pc = (d << 8) | (dd << 4) | ddd;
            break;
        case 0x03:
            if(v[d] == (dd + ddd))
                pc += 2;
            break;
        case 0x04:
            if(v[d] != (dd + ddd))
                pc += 2;
            break;
        case 0x05:
            if(v[d] == v[dd])
                pc += 2;
            break;
        case 0x06:
            v[d] = dd + ddd;
            pc += 2;
            break;
        case 0x07:
            v[d] = v[d] + (dd + ddd);
            pc += 2;
            break;
        case 0x08:
            eightInstructions(d, dd, ddd);
            pc += 2;
            break;
        case 0x09:
            if(v[d] != v[dd])
                pc += 2;
            break;
        case 0x0A:
            i =  (d << 8) | (dd << 4) | ddd;
            pc += 2;
            break;
        case 0x0B:
            i = ((d << 8) | (dd << 4) | ddd) + v[0];
            pc += 2;
            break;
        case 0x0C:
            cOperation(d, dd, ddd);
            pc += 2;
            break;
        case 0x0D:
            v[0x0F] = 0x00;
            writeSprite(v[d], v[dd], ddd);
            pc += 2;
            break;
        case 0x0F:
            fInstructions((d + dd), ddd);
            pc += 2;
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
            v[vx] = v[vx] | v[vy];
            break;
        case 0x02:
            v[vx] = v[vx] & v[vy];
            break;
        case 0x03:
            v[vx] = v[vx] ^ v[vy];
            break;
        case 0x04:
            sum(vx, vy);
            break;
        case 0x05:
            sub(vx, vy);
            break;
        case 0x06:
            break;
        case 0x0E:
            break;
        default:
            std::cerr << "Invalid ArithmeticOperation!!!" << std::endl;
            break;
    }
}

void chip8::printRegister() {
    for (int j = 0; j < 16; j++){
        std::cout << "v[" << j << "]: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(v[j]) << std::endl;
    }
    for (int j = 0; j <= 16; j++){
        std::cout << "stack[" << j << "]: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(stack[j]) << std::endl;
    }
    std::cout << "i: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(i) << std::endl;
}

void chip8::sum(unsigned char vx, unsigned char vy) {
    unsigned short data = v[vx] + v[vy];
    if(data > 0xFF)
        v[0x0F] = 1;
    v[0x0F] = 0;
    v[vx] = data;
}

void chip8::sub(unsigned char vx, unsigned char vy) {
    unsigned short data = v[vx] - v[vy];
    if(data > v[vx])
        v[0x0F] = 0;
    v[0x0F] = 1;
    v[vx] = data;
}

void chip8::cOperation(unsigned char r, unsigned char n, unsigned char nn) {
    unsigned char data = n + nn;
    v[r] = data & randomNumber();
}

unsigned char chip8::randomNumber() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 255);
    return distrib(gen);
}

void chip8::fInstructions(unsigned char opcode, unsigned char address) {
    switch (opcode) {
        case 0x07:
            v[address] = delay;
            break;
        case 0x15:
            delay = v[address];
            break;
        case 0x18:
            sound = v[address];
            break;
        case 0x1E:
            i += v[address];
            break;
        default:
            std::cerr << "Invalid Others Instructions!!!" << std::endl;
            break;
    }
}

void chip8::writeSprite(unsigned char x, unsigned char y, int height) {
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

void chip8::initMemory(const std::vector<unsigned char> &rom) {
    loadFont();
    loadROM(rom);
}