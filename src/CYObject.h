#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include <vector>

enum class ObjectID : uint8_t {
    Chaser		    = 0,
    Crumbs		    = 1,
    DiaPlatform		= 2,
    Diamond		    = 3,
    Door		    = 4,
    Finish		    = 5,
    Floor		    = 6,
    Fuel		    = 7,
    Hole		    = 8,
    Iceman		    = 9,
    JetPack		    = 10,
    Key		        = 11,
    Ladder		    = 12,
    Message		    = 13,
    Pillar		    = 14,
    Platform	    = 15,
    Portal		    = 16,
    Ramp		    = 17,
    Shield		    = 18,
    Slingshot	    = 19,
    Start		    = 20,
    Teleport	    = 21,
    TriPlatform 	= 22,
    TriWall		    = 23,
    Wall		    = 24,
    
    Theme		    = 250,
    Weather		    = 251,
    Music		    = 252,
    Unknown		    = 253,
};

/**
 * @brief X/Z coordinates
 * 
 */
struct Position {
    int16_t x = 0;
    int z = 0;

    int16_t serialize() {

    }
};

/**
 * @brief Represents a single CY Object
 * 
 */
struct CYObject {
    Position position; 
    std::vector<std::string> properties;
    uint8_t floor;

    void verifyPropertyCount(ObjectID id);
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

using CYObjectMap = std::unordered_map<ObjectID, std::vector<CYObject>>;
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