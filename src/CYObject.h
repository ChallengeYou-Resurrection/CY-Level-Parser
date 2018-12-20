#pragma once

#include <string>

struct Position {
    int x = 0;
    int y = 0;
};

struct CYObject {
    Position position; 
    std::string properties;
    uint8_t floor;
};