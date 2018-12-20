#pragma once

#include "CYObject.h"

/**
 * @brief Parses the CY Level format to extract the data from it
 * 
 * @param fileName The name of the .cy file to parse
 * @return CYLevel A map containing all the information about the CYObjects in this level
 */
CYLevel parseFile(const char* fileName);