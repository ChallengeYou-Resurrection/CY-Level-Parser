#pragma once

#include <optional>
#include "CYObject.h"

/**
 * @brief Parses the CY Level format to extract the data from it
 * 
 * @param fileName The name of the .cy file to parse
 * @return CYLevel A map containing all the information about the CYObjects in this level
 */
std::optional<CYLevel> parseFile(const char* fileName);

/**
 * @brief prints out all the errors/ unparsable levels found while parsing
 * 
 */
void printErrors();