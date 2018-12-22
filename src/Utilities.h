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
 * @brief Gets the position of the first instance of 'token' in 'string'
 *
 * @param string The string to search for the 'token' in
 * @param token The token to find the index of
 * @return std::optional<size_t> The position of the token 
 */
std::optional<size_t> indexOf(const std::string& string, char token);

/**
 * @brief Splits a string at a deliminator into seperate tokens
 * 
 * @param string The string to split
 * @param deliminator Character to split the string at
 * @return std::vector<std::string> The tokens
 */
template<typename Iterable>
std::vector<std::string> split(const Iterable& string, char deliminator) {
    std::vector<std::string> tokens;
    std::string token = "";
    for (auto c : string) {
        if (c == deliminator) {
            tokens.push_back(std::move(token));
            continue;
        }
        token.push_back(c);
    }
    tokens.push_back(std::move(token));
    return tokens;
}

/**
 * @brief Concatenate two vectors by performing a move operation on a source vector
 * 
 * @tparam T The type both vectors hold
 * @param destination The std::vector<T> to move the data to
 * @param source The std::vector<T> to move the data from
 * @param offset The offset from the start of the source vector to begin concatenation from (optional)
 */
template<typename T>
void concatenateMoveVector(std::vector<T>& destination, std::vector<T>& source, size_t offset = 0) {
    destination.insert(
        destination.end(),
        std::make_move_iterator(source.begin() + offset),
        std::make_move_iterator(source.end())
    );
}

