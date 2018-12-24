#include "TextureConverter.h"

#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <iostream>

#include "Utilities.h"

namespace {
    using ConvertFunction = std::function<int32_t(const std::string&)>;

    bool isColourTexture(const std::string& texture) {
        return texture.find("color") != std::string::npos;
    }

    int32_t getColourInteger(const std::string& texture) {
        auto colours = split(texture, ',');
        colours[0] = colours[0].substr(6);
        colours[2].pop_back();

        auto r = std::stoi(colours[0]);
        auto g = std::stoi(colours[1]);
        auto b = std::stoi(colours[2]);
        
        return r << 24 | g << 16 | b;
    }

    int32_t convertWallTexture(const std::string& texture) {
        if (isColourTexture(texture)) {
            return getColourInteger(texture);
        } 

        return TextureID::None;
    }

    int32_t convertFloorTexture(const std::string& texture) {
        if (isColourTexture(texture)) {
            return getColourInteger(texture);
        } 

        return TextureID::None;
    }
}

bool hasTexture(const std::string& texture) {
    static const std::unordered_set<const std::string> objectsWithTextures = {
        "walls",
        "floor",
        "plat",
        "triplat",
        "diaplat",
        "ramp",
        "triwall",
        "pillar",
        "door",
    };
    return objectsWithTextures.find(texture) != objectsWithTextures.end();
}


int32_t convertTexture(const std::string& object, const std::string& tex) {
    const static std::unordered_map<const std::string, ConvertFunction> converter {
        {"walls", &convertWallTexture},
        {"floor", &convertFloorTexture},
    };

    return converter.at(object)(tex);
}