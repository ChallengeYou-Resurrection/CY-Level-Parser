#include "FileParser.h"
#include "Utilities.h"

#include <iostream>
#include <algorithm>
#include <stack>

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
    auto content = getFileContent(fileName);
    content.pop_back();
    auto tokens = split(content, '#');

    level.name      = getMetadataAttribute("name", tokens[1], true);
    level.numFloors = getMetadataAttribute("levels", tokens[2], false);
    level.version   = getMetadataAttribute("version", tokens[3], false);
    level.creator   = getMetadataAttribute("creator", tokens[4], true);

    //Offset 7 to ignore metadata + floors and walls (special case objects)
    std::for_each(tokens.cbegin() + 7, tokens.cend(), [&level](const std::string& token) {
        //Find the name of this object
        auto nameEndIndex = indexOf(token, ':');
        std::string_view objectName (token.c_str(), *nameEndIndex);
        std::string_view data       (token.c_str() + *nameEndIndex + 2);

        //Match the square brackets
        std::vector<std::pair<std::size_t, std::size_t>> sections;
        std::stack<size_t> unmatchedIndices;
        for (size_t i = 0; i < data.length(); i++) {
            auto c = data[i];
            if (c == '[') {
                unmatchedIndices.push(i);
            }
            else if (c == ']') {
                auto begin  = unmatchedIndices.top();
                auto length = i - begin;
                unmatchedIndices.pop();
                sections.emplace_back(begin + 1, length - 1);
            }
        }

        //Extraction of the data
        const auto& s = sections;
        const auto& d = data;      
        std::vector<CYObject> objects; 
        for (size_t i = 0; i < s.size() - 1; i += 3) {
            CYObject object;
            std::string_view position   (d.data() + s[i].first, s[i].second);
            std::string_view properties (d.data() + s[i + 1].first, s[i + 1].second);
            std::string_view fullData   (d.data() + s[i + 2].first, s[i + 2].second);
            
            auto pos = split(position, ',');
            auto x = std::stoi(pos[0]);
            auto y = std::stoi(pos[1]);
            auto floor = std::stoi(split(fullData, ',').back());

            object.position     = {x, y};
            object.properties   = properties.data();
            object.floor        = floor;
        }
        level.objects.emplace(std::string(objectName.data()), std::move(objects));
    });
    
    return level;
}