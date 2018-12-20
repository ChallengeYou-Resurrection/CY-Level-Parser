#pragma once

#include <unordered_map>
#include <vector>

#include "CYObject.h"

/**
 * @brief Type alias for a map containing information about all the objects in a level
 */
using CYObjectMap = std::unordered_map<std::string, std::vector<CYObject>>;

/**
 * @brief Parses the CY Level format to extract the data from it
 * 
 * @param fileName The name of the .cy file to parse
 * @return CYObjectMap A map containing all the information about the CYObjects in this level
 */
CYObjectMap parseFile(const char* fileName);