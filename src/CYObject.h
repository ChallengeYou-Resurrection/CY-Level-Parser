#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include <vector>

enum class ObjectID : uint8_t {
    Wall        = 0,
    Floor       = 1,
    Platform    = 2,
    TriPlatform = 3,
    DiaPlatform = 4,
    Ramp        = 5,
    TriWall     = 6,
    Pillar      = 7,
    Door        = 8,
    Diamond     = 9,
    Iceman      = 10,
    Chaser      = 11,
    Hole        = 12,
    Start       = 13,
    Finish      = 14,
    JetPack     = 15,
    Fuel        = 16,
    Shield      = 17,
    Slingshot   = 19,
    Crumbs      = 20,
    Teleport    = 21,
    Key         = 22,
    Ladder      = 23,
    Portal      = 24,    
    Message     = 25,

    Music       = 250,
    Weather     = 251,
    Theme       = 252,
};

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

    void verifyPropertyCount(const std::string& name);
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

    void verifyPropertyCount();
};

//@TODO Change back to 
using CYObjectMap = std::map<std::string, std::vector<CYObject>>;
struct CYLevel {
    std::string name;
    std::string creator;
    std::string version;
    std::string numFloors;
    int         backmusic   = 1;
    int         theme       = 1;
    int         weather     = 1;

    CYObjectMap objects;
    std::vector<CYFloor> floors;
    std::vector<CYWall> walls;
};

ObjectID stringToObjectID(const std::string& objectName);