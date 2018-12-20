#pragma once

#include <string>
#include <vector>
#include <optional>

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
 * @param string The string to split
 * @param deliminator Character to split the string at
 * @return std::vector<std::string> The tokens
 */
std::vector<std::string> split(const std::string& string, char deliminator);


/**
 * @brief Gets the position of the first instance of 'token' in 'string'
 *
 * @param string The string to search for the 'token' in
 * @param token The token to find the index of
 * @return std::optional<size_t> The position of the token 
 */
std::optional<size_t> indexOf(const std::string& string, char token);