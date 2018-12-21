#pragma once

#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief X/Z coordinates
 * 
 */
struct Position {
    int x = 0;
    int z = 0;
};

/**
 * @brief Represents a single CY Object
 * 
 */
struct CYObject {
    Position position; 
    std::string properties;
    uint8_t floor;
};

struct CYFloor {
    Position vertexA;
    Position vertexB;
    Position vertexC;
    Position vertexD;
    std::string properties;
    uint8_t floor;
};

struct CYWall {
    Position a;
    Position b;
    std::string properties;
    uint8_t floor;
};

using CYObjectMap = std::unordered_map<std::string, std::vector<CYObject>>;
struct CYLevel {
    std::string name;
    std::string creator;
    std::string version;
    std::string numFloors;

    CYObjectMap objects;
    std::vector<CYFloor> floors;
    std::vector<CYWall> walls;
};