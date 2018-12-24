#pragma once

#include <string>

enum class TextureID : uint8_t
{
	// Normal Textures
	Color = 0,
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
    None,
};

bool hasTexture(const std::string& texture);
TextureID convertTexture(const std::string& object, const std::string& texutre);