#pragma once

#include <unordered_map>
#include <vector>

#include "CYObject.h"

using CYObjectMap = std::unordered_map<std::string, std::vector<CYObject>>;

CYObjectMap parseFile(const char* fileName);