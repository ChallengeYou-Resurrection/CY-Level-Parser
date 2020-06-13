#include <iostream>
#include <fstream>
#include <sstream>

#include "Utilities.h"

size_t getFileLength(const std::string& fileName) {
    return std::ifstream(fileName, std::ifstream::binary | std::ifstream::ate).tellg();
}

std::string getFileContent(const std::string& fileName) {
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

size_t findIgnoreQuotes(const std::string& str, const std::string& search) {
    bool isInQuotes = false;
    std::string buffer;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '\"') {
            buffer.clear();
            isInQuotes = !isInQuotes;
            continue;
        }
        if (str[i] == search[buffer.length()] && !isInQuotes) {
            buffer.push_back(str[i]);
            if (buffer.length() == search.length()) {
                return i - search.length() + 1;
            }
        }
        else {
            buffer.clear();
        }
    }
    return std::string::npos;
}