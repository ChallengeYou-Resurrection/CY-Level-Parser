#pragma once

#include <string>
#include <memory>
#include "CYEnums.h"

struct Texture {
    virtual std::string asString() = 0;
};

struct TextureNormal : public Texture {
    TextureNormal(int textureId)
        : id(textureId) 
    {}

    int id;

    std::string asString() override;
};

struct TextureColour : public Texture {
    TextureColour(int red, int green, int blue)
        : r(red)
        , g(green)
        , b(blue)
    {}

    int r;
    int g;
    int b;

    std::string asString() override;
};

bool hasTexture(ObjectID id);
std::unique_ptr<Texture> convertTexture(ObjectID object, const std::string& texture);