//
// Created by Bruno Augusto on 05/09/24.
//

#include "file.h"
#include <iostream>
#include <fstream>

int file::getFileBytes(std::vector<unsigned char> &fileV, const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if(!file.is_open()){
        std::cerr << "Error to open file" << std::endl;
        return 1;
    }

    unsigned char byte;
    while (file.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        fileV.push_back(byte);
    }

    file.close();

    return 0;
}
