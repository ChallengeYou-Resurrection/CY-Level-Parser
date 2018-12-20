#include "FileParser.h"
#include "Utilities.h"

#include <iostream>

namespace {
    std::string getMetadataAttribute(
        const std::string& name, const std::string& token, bool isString) {
        return token.substr(
            name.length() + (isString ? 3 : 2),
            token.length() - name.length() - (isString ? 6 : 4));
    }
}
CYObjectMap parseFile(const char* fileName) {
    CYObjectMap objects;
    auto tokens = split(getFileContent(fileName), '#');

    auto name       = getMetadataAttribute("name", tokens[1], true);
    auto floors     = getMetadataAttribute("levels", tokens[2], false);
    auto version    = getMetadataAttribute("version", tokens[3], false);
    auto creator    = getMetadataAttribute("creator", tokens[4], true);
    
    return objects;
}