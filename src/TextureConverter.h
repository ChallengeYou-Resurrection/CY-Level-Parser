#pragma once

#include <string>
#include <memory>
#include "CYEnums.h"

struct Texture {
    int id;
    std::string colour = "FFFFFF";
};

bool hasTexture(ObjectID id);
Texture convertTexture(ObjectID object, const std::string& texture);