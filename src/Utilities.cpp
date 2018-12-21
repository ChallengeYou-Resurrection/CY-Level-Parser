#include <iostream>
#include <fstream>
#include <sstream>

#include "Utilities.h"

size_t getFileLength(const char* fileName) {
    return std::ifstream(fileName, std::ifstream::binary | std::ifstream::ate).tellg();
}

std::string getFileContent(const char* fileName) {
    std::fstream inFile(fileName);
    if (!inFile.is_open()) return "";
    std::string fileContent;
    auto length = getFileLength(fileName);
    fileContent.reserve(length);
    std::getline(inFile, fileContent);
    return fileContent;
}


std::optional<size_t> indexOf(const std::string& string, char token) {
    for (size_t i = 0; i < string.length(); i++) {
        if (string[i] == token) {
            return i;
        }
    }
    return {};
}