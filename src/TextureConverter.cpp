#include "TextureConverter.h"

#include <unordered_set>


bool hasTexture(const std::string& texture) {
    static const std::unordered_set<std::string> objectsWithTextures = {
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


TextureID convertTexture(const std::string& object, const std::string& tex) {


    return TextureID::None;
}