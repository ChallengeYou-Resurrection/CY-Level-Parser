#include "TextureConverter.h"

#include <bitset>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "Utilities.h"

namespace {
    bool isColourTexture(const std::string& texture)
    {
        return texture.find("color") != std::string::npos;
    }

    std::string colourToString(const std::string& texture)
    {
        auto colours = split(texture, ',');
        colours[0] = colours[0].substr(6);
        colours[2].pop_back();
        auto r = std::stoi(colours[0]);
        auto g = std::stoi(colours[1]);
        auto b = std::stoi(colours[2]);

        char hex[16];
        snprintf(hex, sizeof hex, "c %02x%02x%02x", r, g, b);

        return hex;

        // for binary fomat only, note: r, g, and b MUST BE CONVERTED TO 8 BIT UNSIGNED
        // FIRST
        // return (r << 24) | (g << 16) | (b << 8) | 0xFF;
    }

    uint32_t convertWallTexture(int texture)
    {
        switch (texture) {
            case 1:
                return TextureID::Brick;
            case 2:
                return TextureID::Bars;
            case 3:
                return TextureID::Stone;
            case 4:
                return TextureID::Grass;
            case 5:
                return TextureID::Wood;
            case 6:
                return TextureID::Happy;
            case 7:
                return TextureID::Egypt;
            case 8:
                return TextureID::Glass;
            case 9:
                return TextureID::Stucco;
            case 10:
                return TextureID::Bark;
            case 11:
                return TextureID::Scifi;
            case 12:
                return TextureID::Tile;
            case 13:
                return TextureID::Rock;
            case 14:
                return TextureID::Books;
            case 16:
                return TextureID::Parquet;
            default:
                return TextureID::None;
        }
    }

    uint32_t convertPlatformTexture(int texture)
    {
        switch (texture) {
            case 1:
                return TextureID::Grass;
            case 2:
                return TextureID::Stucco;
            case 3:
                return TextureID::Brick;
            case 4:
                return TextureID::Stone;
            case 5:
                return TextureID::Wood;
            case 6:
                return TextureID::Happy;
            case 7:
                return TextureID::Egypt;
            case 8:
                return TextureID::Glass;
            case 9:
                return TextureID::Bark;
            case 10:
                return TextureID::Scifi;
            case 11:
                return TextureID::Tile;
            case 13:
                return TextureID::Rock;
            case 14:
                return TextureID::Parquet;

            default:
                return TextureID::None;
        }
    }
} // namespace

///@TODO Replace with switch case for ObjectID
bool hasTexture(ObjectID id)
{
    switch (id) {
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

Texture convertTexture(ObjectID object, const std::string& format)
{
    Texture texture;
    if (isColourTexture(format)) {
        texture.id = TextureID::Colour;
        texture.colour = colourToString(format);
    }

    switch (object) {
        case ObjectID::Wall:
        case ObjectID::Pillar:
        case ObjectID::Door:
        case ObjectID::TriWall:
            texture.id = convertWallTexture(std::stoul(format));
            break;

        case ObjectID::Floor:
        case ObjectID::Platform:
        case ObjectID::TriPlatform:
        case ObjectID::DiaPlatform:
        case ObjectID::Ramp:
            texture.id = convertPlatformTexture(std::stoul(format));
            break;

        default:
            std::cerr << "UNKNOWN OBEJCT";
            texture.id = TextureID::Unknown;
    }

    return texture;
}