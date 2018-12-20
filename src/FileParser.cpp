#include "FileParser.h"
#include "Utilities.h"

#include <iostream>
#include <algorithm>

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

    //Offset 7 to ignore metadata + floors and walls (special case objects)
    std::for_each(tokens.cbegin() + 7, tokens.cend(), [level](const std::string& token) {
        //Find the name of this object
        auto nameEndIndex = indexOf(token, ':');
        //if (!nameEndIndex) return;
        std::string_view objectName (token.c_str(), *nameEndIndex);
        std::string_view data       (token.c_str() + *nameEndIndex + 2);

        std::cout << objectName << " DATA~" << data << "~\n";
    });
    
    return level;
}