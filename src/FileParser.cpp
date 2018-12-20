#include "FileParser.h"
#include "Utilities.h"

#include <iostream>

CYObjectMap parseFile(const char* fileName) {
    CYObjectMap objects;
    auto content = getFileContent(fileName);

    auto tokens = split(content, '#');
    for (auto& t : tokens) {
        std::cout << t << '\n';
    }
    
    return objects;
}