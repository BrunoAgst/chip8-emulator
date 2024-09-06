//
// Created by Bruno Augusto on 04/09/24.
//
#include <vector>

#ifndef CHIP_8_CHIP8_H
#define CHIP_8_CHIP8_H


class chip8 {
public:
    unsigned char chip8ScreenBuffer[64][32] = {0};
    bool drawFlag = false;
    void initMemory(const std::vector<unsigned char>& rom);
    void execute();
    void printRegister();
private:
    unsigned char memory[4096]{0x00};
    unsigned char v[15]{};
    unsigned short stack[16]{0x00};
    unsigned short pc = 0x200;
    unsigned char sp = 0x00;
    unsigned short i = 0x00;
    unsigned char sound = 0x00;
    unsigned char delay = 0x00;
    static unsigned char randomNumber();
    void writeSprite(unsigned char x, unsigned char y, int height);
    void eightInstructions(unsigned char vx, unsigned char vy, unsigned char opcode);
    void selectInstruction(unsigned char opcode, unsigned char d, unsigned dd, unsigned char ddd);
    void sum(unsigned char vx, unsigned char vy);
    void cOperation(unsigned char r,unsigned char n, unsigned char nn);
    void sub(unsigned char vx, unsigned char vy);
    void fInstructions(unsigned char opcode, unsigned char address);
    void loadROM(const std::vector<unsigned char>& rom);
    void loadFont();
};


#endif //CHIP_8_CHIP8_H
