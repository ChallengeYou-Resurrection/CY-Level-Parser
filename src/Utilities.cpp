#include <iostream>
#include <fstream>
#include <sstream>

size_t getFileLength(const char* fileName) {
    return std::ifstream(fileName, std::ifstream::binary | std::ifstream::ate).tellg();
}

std::string getFileContent(const char* fileName) {
    std::fstream inFile(fileName);
    std::string fileContent;
    fileContent.reserve(getFileLength(fileName));
    std::getline(inFile, fileContent);
    return fileContent;
}