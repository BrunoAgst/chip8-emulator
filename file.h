//
// Created by Bruno Augusto on 05/09/24.
//
#include <vector>

#ifndef CHIP_8_FILE_H
#define CHIP_8_FILE_H


class file {
public:
    static int getFileBytes(std::vector<unsigned char> &file, const std::string& filePath);
};


#endif //CHIP_8_FILE_H
