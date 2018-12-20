#pragma once

#include <string>
#include <vector>

/**
 * @brief Get the length of a file, useful for optimization purposes for allocating
 * memory on a string
 *
 * @param fileName The name of the file to get the length of
 * @return size_t The length of the file
 */
size_t getFileLength(const char* fileName);

/**
 * @brief Get the contents from a file
 * 
 * @param fileName The name of the file to get the contents from
 * @return std::string String containing everything inside of this file
 */
std::string getFileContent(const char* fileName);

/**
 * @brief Splits a string at a deliminator into seperate tokens
 * 
 * @param deliminator Character to split the string at
 * @return std::vector<std::string> The tokens
 */
std::vector<std::string> split(char deliminator);