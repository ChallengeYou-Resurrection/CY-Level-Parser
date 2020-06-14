#pragma once

#include <string>

struct CYLevel;

void writeLevelBinary(const CYLevel& level, const std::string& fileName);
void writeLevelJson(const CYLevel& level, const std::string& fileName);