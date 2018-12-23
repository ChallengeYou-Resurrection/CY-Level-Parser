#pragma once

#include <string>
#include <unordered_map>
#include <map>
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
    std::vector<std::string> properties;
    uint8_t floor;
};

struct CYFloor {
    Position vertexA;
    Position vertexB;
    Position vertexC;
    Position vertexD;
    std::vector<std::string> properties;
    uint8_t floor;
};

struct CYWall {
    Position beginPoint;
    Position endPoint;
    std::vector<std::string> properties;
    uint8_t floor;
};

//@TODO Change back to 
using CYObjectMap = std::map<std::string, std::vector<CYObject>>;
struct CYLevel {
    std::string name;
    std::string creator;
    std::string version;
    std::string numFloors;

    CYObjectMap objects;
    std::vector<CYFloor> floors;
    std::vector<CYWall> walls;
};