#include "FileParser.h"
#include "Utilities.h"

#include <iostream>
#include <algorithm>
#include <stack>

/**
 * @brief This pair defines a pair of matching square brackets
 * 
 */
using BracketMatch = std::pair<std::size_t, std::size_t>;

namespace {
    std::string getMetadataAttribute(
        const std::string& name, const std::string& token, bool isString) {
        return token.substr(
            name.length() + (isString ? 3 : 2),
            token.length() - name.length() - (isString ? 6 : 4));
    }

    Position extractPosition(const std::string_view& v) {
        auto pos = split(v, ',');
        auto x = std::stoi(pos[0]);
        auto z = std::stoi(pos[1]);
        return {x, z};
    }

    std::string_view getMatchSection(const BracketMatch& match, const std::string& data) {
        return std::string_view(data.data() + match.first, match.second);
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


    std::for_each(tokens.cbegin() + 5, tokens.cend(), [&level](const std::string& token) {
        //Find the name of this object
        auto nameEndIndex = indexOf(token, ':');
        auto objectName = token.substr(0, *nameEndIndex);
        auto data       = token.substr(*nameEndIndex + 2);

        //Match the square brackets [ .. ]
        std::vector<BracketMatch> sections;
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
        

        //Walls and floors have a special format, everything else follows the same format
        //[[x, y], [properties], floor]
        const auto& s = sections;
        const auto& d = data;    
        if (objectName == "Floor") {
            std::vector<CYFloor> floors;

            for (size_t i = 0; i < s.size() - 1; i += 8) {
                CYFloor floor;
                floor.vertexA = extractPosition(getMatchSection(s[i    ], d));
                floor.vertexB = extractPosition(getMatchSection(s[i + 1], d));
                floor.vertexC = extractPosition(getMatchSection(s[i + 2], d));
                floor.vertexD = extractPosition(getMatchSection(s[i + 3], d));
                floor.properties = d.substr(s[i + 6].first, s[i + 6].second);
                floor.floor = i % 8;
                floors.push_back(floor);
            }
            level.floors = std::move(floors);
        }
        else if (objectName == "walls") {
            std::vector<CYWall> walls;
            for (size_t i = 0; i < s.size() - 1; i += 2) {
                auto tokens = split(d.substr(s[i + 1].first, s[i + 2].second), ',');
                auto properties = getMatchSection(s[i], d);

                CYWall wall;

                //...
                walls.push_back(wall);
            }
            level.walls = std::move(walls);
        }
        else {
            //Extraction of the data     
            std::vector<CYObject> objects; 
            for (size_t i = 0; i < s.size() - 1; i += 3) {
                auto fullData = getMatchSection(s[i + 2], d);

                CYObject object;
                object.position     = extractPosition(getMatchSection(s[i], d));
                object.properties   = d.substr( s[i + 1].first, s[i + 1].second);
                object.floor        = std::stoi(split(fullData, ',').back());
                objects.push_back(object);
            }
            level.objects.emplace(std::string(objectName.data()), std::move(objects));
        }
    });
    
    return level;
}