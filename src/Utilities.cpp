#include <iostream>
#include <fstream>
#include <sstream>

#include "Utilities.h"

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

std::vector<std::string> split(const std::string& string, char deliminator) {
    std::vector<std::string> tokens;
    std::string token = "";
    for (auto c : string) {
        if (c == deliminator) {
            tokens.push_back(std::move(token));
            continue;
        }
        token.push_back(c);
    }
    return tokens;
}

std::optional<size_t> indexOf(const std::string& string, char token) {
    for (size_t i = 0; i < string.length(); i++) {
        if (string[i] == token) {
            return i;
        }
    }
    return {};
}