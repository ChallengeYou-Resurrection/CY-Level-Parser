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
CYLevel parseFile(const char* fileName) {
    CYLevel level;
    auto tokens = split(getFileContent(fileName), '#');

    level.name      = getMetadataAttribute("name", tokens[1], true);
    level.numFloors = getMetadataAttribute("levels", tokens[2], false);
    level.version   = getMetadataAttribute("version", tokens[3], false);
    level.creator   = getMetadataAttribute("creator", tokens[4], true);
    
    return level;
}