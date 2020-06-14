#pragma once

#include <cstdint>

enum class ObjectID : uint8_t {
    Chaser = 0,
    Crumbs = 1,
    DiaPlatform = 2,
    Diamond = 3,
    Door = 4,
    Finish = 5,
    Floor = 6,
    Fuel = 7,
    Hole = 8,
    Iceman = 9,
    JetPack = 10,
    Key = 11,
    Ladder = 12,
    Message = 13,
    Pillar = 14,
    Platform = 15,
    Portal = 16,
    Ramp = 17,
    Shield = 18,
    Slingshot = 19,
    Start = 20,
    Teleport = 21,
    TriPlatform = 22,
    TriWall = 23,
    Wall = 24,
    NUM_OBJECTS = 25,

    Theme = 250,
    Weather = 251,
    Music = 252,
    Unknown = 253,
};


//This is not an enum class because of colour textures are represented using 32bits for RGBT
//where T is just 255 in binary to ID this is a texure
//Layout for colours: RRRRRRRRGGGGGGGGBBBBBBBB11111111
enum TextureID : uint32_t {
    // Normal Textures
    Colour = 0,
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
