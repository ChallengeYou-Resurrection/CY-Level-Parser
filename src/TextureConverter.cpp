#include "TextureConverter.h"

#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <iostream>

#include "Utilities.h"

namespace {
    using ConvertFunction = std::function<int32_t(int)>;

    bool isColourTexture(const std::string& texture) {
        return texture.find("color") != std::string::npos;
    }

    int32_t colourToInt(const std::string& texture) {
        auto colours = split(texture, ',');
        colours[0] = colours[0].substr(6);
        colours[2].pop_back();

        auto r = std::stoi(colours[0]);
        auto g = std::stoi(colours[1]);
        auto b = std::stoi(colours[2]);
        
        return r << 16 | g << 8 | b;
    }

    int32_t convertWallTexture(int texture) {
        switch (texture) {
            case 1: return  TextureID::Brick;
            case 2: return  TextureID::Bars;
            case 3: return  TextureID::Stone;
            case 4: return  TextureID::Grass;
            case 5: return  TextureID::Wood;
            case 6: return  TextureID::Happy;
            case 7: return  TextureID::Egypt;
            case 8: return  TextureID::Glass;
            case 9: return  TextureID::Stucco;
            case 10: return TextureID::Bark;
            case 11: return TextureID::Scifi;
            case 12: return TextureID::Tile;
            case 13: return TextureID::Rock;
            case 14: return TextureID::Books;
            case 16: return TextureID::Parquet;
            default: return TextureID::None;
        }
    }

    int32_t convertFloorTexture(int texture) {

        return TextureID::None;
    }

    int32_t convertPlatformTexture(int texture) {
        switch (texture) // WALLS
        {
            case 1: return  TextureID::Grass;
            case 2: return  TextureID::Stucco;
            case 3: return  TextureID::Brick;
            case 4: return  TextureID::Stone;
            case 5: return  TextureID::Wood;
            case 6: return  TextureID::Happy;
            case 7: return  TextureID::Egypt;
            case 8: return  TextureID::Glass;
            case 9: return  TextureID::Bark;
            case 10: return TextureID::Scifi;
            case 11: return TextureID::Tile;
            case 13: return TextureID::Rock;
            case 14: return TextureID::Parquet;

            default: return TextureID::None;
        }
    }
}

///@TODO Replace with switch case for ObjectID
bool hasTexture(ObjectID id) {
    switch(id) {
        case ObjectID::Platform:
        case ObjectID::TriPlatform:
        case ObjectID::DiaPlatform:
        case ObjectID::Ramp:
        case ObjectID::TriWall:
        case ObjectID::Pillar:
        case ObjectID::Door:
            return true;

        default:
            return false;
    }
}


int32_t convertTexture(ObjectID object, const std::string& texture) {
    if (isColourTexture(texture)) {
        return colourToInt(texture);
    }

    switch(object) {
        case ObjectID::Wall:
        case ObjectID::Pillar:
        case ObjectID::Door:
        case ObjectID::TriWall:
            return convertWallTexture(std::stoi(texture));

        case ObjectID::Floor:
        case ObjectID::Platform:
        case ObjectID::TriPlatform:
        case ObjectID::DiaPlatform:
        case ObjectID::Ramp:
            return convertFloorTexture(std::stoi(texture));

        default:
            std::cerr << "UNKNOWN OBEJCT";
            return -1;
    }

}