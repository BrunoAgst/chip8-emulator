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
    void initCPU(const std::vector<unsigned char>& rom);
    void execute();
private:
    unsigned short stack[16]{0x00};
    unsigned short i = 0x00;
    unsigned short pc = 0x200;
    unsigned char memory[4096]{0x00};
    unsigned char v[15]{0x00};
    unsigned char sound = 0x00;
    unsigned char delay = 0x00;
    unsigned char sp = 0x00;
    unsigned int keyMapping[16]{0x00};
    bool keyState[16]{false};
    static unsigned char randomNumber();
    void writeSprite(unsigned char x, unsigned char y, int height);
    void eightInstructions(unsigned char vx, unsigned char vy, unsigned char opcode);
    void selectInstruction(unsigned char opcode, unsigned char d, unsigned char dd, unsigned char ddd);
    void sum(unsigned char vx, unsigned char vy);
    void cOperation(unsigned char r,unsigned char data);
    void sub(unsigned char vx, unsigned char vy);
    void fInstructions(unsigned char x, unsigned char opcode);
    void loadROM(const std::vector<unsigned char>& rom);
    void loadFont();
    void bitShiftRight(unsigned char vx);
    void bitShiftLeft(unsigned char vx);
    void subShift(unsigned char vx, unsigned char vy);
    void fInstruction33(unsigned char vx);
    void fInstruction55(unsigned char vx);
    void fInstruction65(unsigned char vx);
    void initKeyboard();
    void waitForPress(unsigned char vx);
    void kInstructions(unsigned char vx, unsigned char opcode);
};


#endif //CHIP_8_CHIP8_H
