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
            token.length() - name.length() - (isString ? 6 : 4)
        );
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

struct Error {
    Error(std::string name, std::string error)
    :   name    (name)
    ,   reason  (error) 
    {}

    std::string name;
    std::string reason;
};
std::vector<Error> errors;

std::optional<CYLevel> parseFile(const char* fileName) {
    CYLevel level;
    auto content = getFileContent(fileName);
    content.pop_back();

#ifdef DEBUG 
    std::cout << "Got the file data.\n";
#endif 

    auto metaDataEndLocation = findIgnoreQuotes(content, "Floor");
    if (metaDataEndLocation == std::string::npos) {
        metaDataEndLocation = findIgnoreQuotes(content, "floor");
    }
    if (metaDataEndLocation == std::string::npos) {
        errors.emplace_back(fileName, "Cannot find #Floor section");
        return {};
    } 
#ifdef DEBUG 
    std::cout << "Found location of floor.\n";
#endif 
    auto metadata = content.substr(0, metaDataEndLocation);
    auto objects  = content.substr(metaDataEndLocation - 1);

#ifdef DEBUG 
    std::cout << "Object and metadata strings have been split.\n";
    std::cout << "Metadata:\n\t" << metadata << '\n';
#endif 

    auto metaDataTokens = split(metadata, '#', true);

    //Extract metadata from the file
    level.name      = getMetadataAttribute("name",    metaDataTokens[1], true);
    level.numFloors = getMetadataAttribute("levels",  metaDataTokens[2], false);
    level.version   = getMetadataAttribute("version", metaDataTokens[3], false);
    level.creator   = getMetadataAttribute("creator", metaDataTokens[4], true);
#ifdef DEBUG 
    std::cout   << "Metadata: \n" 
                << "\tName:    " << level.name      << '\n'
                << "\tFloors:  " << level.numFloors << '\n'
                << "\tVersion: " << level.version   << '\n'
                << "\tCreator: " << level.creator   << '\n';
#endif 

    if (level.version[0] == '1') {
        errors.emplace_back(fileName, "Version 1");
        return {};
    }
    if (stof(level.version) < 2.1) {
        errors.emplace_back(fileName, "Version older than 2.1");
        return {};
    }

    //@TODO Combine with function in loop below
    std::vector<std::pair<std::string, std::string>> tokens;
    bool isInsideString = false;
    std::stack<size_t> unmatchedIndices;
    std::string name;
    for (size_t i = 0; i < objects.length(); i++) {
        auto c = objects[i];
        if (unmatchedIndices.empty()) {
            name.push_back(c);
        }
        if (c == '[' && !isInsideString) {
            unmatchedIndices.push(i);
        }
        else if (c == ']' && !isInsideString) {
            auto begin  = unmatchedIndices.top();
            auto length = i - begin;
            unmatchedIndices.pop();
            if (unmatchedIndices.empty()) {
                name.erase(
                    std::remove_if(name.begin(), name.end(), [](char c) {
                        return c == ',' || c =='#' || c == ':' || c == '[' || c == '\"' ||std::isspace(c);
                    }),
                    name.end());
                    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
                tokens.emplace_back(std::move(name), objects.substr(begin, length + 1));
            }
        }
        else if (c == '\"') {
            isInsideString = !isInsideString;
        }
    }

#ifdef DEBUG
    for (const auto& token : tokens) {
        std::cout << "Token: <" << token.first << ">\n";
        std::cout << "\tData: " << token.second << "\n";
    }
#endif

    //Extraction of all the data objcts
#ifdef DEBUG
    std::cout << "\n\n Begin data extraction\n\n";
#endif
    for (const auto& tokenPair : tokens) {
        const auto& objectName = tokenPair.first;
        const auto& data       = tokenPair.second;
#ifdef DEBUG
        std::cout << "Parsing: <" << objectName << ">" << std::endl;
#endif
        //Match the square brackets [ .. ]
        std::vector<BracketMatch> sections;
        std::stack<size_t> unmatchedIndices;
        bool isInsideString = false;
        for (size_t i = 0; i < data.length(); i++) {
            auto c = data[i];
            if (c == '[' && !isInsideString) {
                unmatchedIndices.push(i);
            }
            else if (c == ']' && !isInsideString) {
                auto begin  = unmatchedIndices.top();
                auto length = i - begin;
                unmatchedIndices.pop();
                sections.emplace_back(begin + 1, length - 1);
            }
            else if (c == '\"') {
                isInsideString = !isInsideString;
            }
        }
        
        // = = = = = = = = = = = = = =
        // = Data extraction section =
        // = = = = = = = = = = = = = =
        //Walls and floors have a special format, everything else follows the same format
        //[[x, y], [properties], floor]
        const auto& s = sections;
        const auto& d = data;    
        if (objectName == "floor") {
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
                if (data.find("<Void>") != std::string::npos) {
                    errors.emplace_back(fileName, "Wall contains '<Void>'");
                    return {};
                }
                auto tokens = split(d.substr(s[i + 1].first, s[i + 1].second), ',');
                auto properties = getMatchSection(s[i], d);

                int xOffset = std::stoi(tokens[0]);
                int zOffset = std::stoi(tokens[1]);

                int xBegin  = std::stoi(tokens[2]);
                int zBegin  = std::stoi(tokens[3]);

                CYWall wall;
                wall.beginPoint = {xBegin,              zBegin};
                wall.endPoint   = {xBegin + xOffset,    zBegin + zOffset};
                wall.properties = properties;
                wall.floor      = std::stoi(tokens.back());
                walls.push_back(wall);
            }
            level.walls = std::move(walls);
        }
        else {    
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
    };

    return level;
}

void printErrors() {
    std::cout << "\n=======================================\n";
    std::cout << "Printing all error levels came across: \n";
    for (const auto& error : errors) {
        std::cout   << "File: " << error.name << "\n"
                    << "\tReason: " << error.reason << "\n\n";
    }
}