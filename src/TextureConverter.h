#pragma once

#include <string>
#include "CYObject.h"

//This is not an enum class because of colour textures are represented using 24bits for RGB
//Layout for colours: 00000000RRRRRRRRGGGGGGGGBBBBBBBB
namespace TextureID {
    enum : uint32_t  {
        // Normal Textures
        Grass = 1,
        Stucco = 2,
        Brick = 3,
        Stone = 4,
        Wood = 5,
        Happy = 6,
        Egypt = 7,
        Bark = 8,
        Scifi = 9,
        Tile = 10,
        Rock = 11,
        Parquet = 12,
        Books = 13,

        // Translucent Textures
        Bars = 14,
        Glass = 15,
        None = 16,
        Unknown = 17
    };
}

bool hasTexture     (ObjectID id);
uint32_t convertTexture  (ObjectID object, const std::string& texture);