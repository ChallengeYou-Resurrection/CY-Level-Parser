#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <memory>
#include <array>

#include "BinaryFile.h"
#include "CYEnums.h"
#include "TextureConverter.h"

/**
 * @brief X/Z coordinates
 * 
 */
struct Position {
    int x = 0;
    int z = 0;

    void serialize(BinaryFileBuffer& buffer) const {
        buffer << (i16)x << (i16)z;
    }
};

struct CYFloor {
    Position vertexA;
    Position vertexB;
    Position vertexC;
    Position vertexD;
    std::unique_ptr<Texture> topTexture;
    std::unique_ptr<Texture> bottomTexture;
    uint8_t floor;
    bool isVisible = true;

    void serialize(BinaryFileBuffer& buffer) const;
};

struct CYWall {
    Position beginPoint;
    Position endPoint;
    std::vector<std::string> properties;
    uint8_t floor;

    std::unique_ptr<Texture> backTexture;
    std::unique_ptr<Texture> frontTexture;


    void serialize(BinaryFileBuffer& buffer) const;
    void verifyPropertyCount();
};

/**
 * @brief Represents a single CY Object
 * 
 */
struct CYObject {
    Position position; 
    uint8_t floor;

    std::unique_ptr<Texture> texture;

    std::vector<std::string> properties;

    void serialize(BinaryFileBuffer& buffer) const;
    void verifyPropertyCount(ObjectID id);
};

using CYObjectMap = std::array<std::vector<CYObject>, (int)ObjectID::NUM_OBJECTS>;
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

    size_t numObjects(ObjectID id) const {
        return objects[(int)id].size();
    }
};

ObjectID stringToObjectID(const std::string& objectName);