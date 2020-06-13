#include "FileParser.h"
#include "Utilities.h"

#include <iostream>
#include <algorithm>
#include <stack>

#include "TextureConverter.h"

/**
 * @brief This pair defines a pair of matching square brackets
 * 
 */
using BracketMatch = std::pair<std::size_t, std::size_t>;

namespace {
    struct Error {
        Error(std::string name, std::string error)
        :   name    (name)
        ,   reason  (error) 
        {}

        std::string name;
        std::string reason;
    };
    std::vector<Error> errors;

    //Get the Metadata Attributes from the header of a level file
    std::string getMetadataAttribute(
        const std::string& name, const std::string& token, bool isString) {
        return token.substr(
            name.length() + (isString ? 3 : 2),
            token.length() - name.length() - (isString ? 6 : 4)
        );
    }

    //Extracts a position from a string
    Position extractPosition(const std::string_view& v) {
        auto pos = split(v, ',');
        auto x = std::stoi(pos[0]);
        auto z = std::stoi(pos[1]);
        return {x, z};
    }

    //Gets a section of the data string between two [ ] as defined by `match` parameter
    std::string_view getMatchSection(const BracketMatch& match, const std::string& data) {
        return std::string_view(data.data() + match.first, match.second);
    }

    //Extracts the properties from a string which does not contain a message
    std::vector<std::string> extractProperties(const std::string& properties) {
        auto propertyList = split(properties, ',', true, '(', ')');
        for (auto& p : propertyList) {
            removeFrom(p, {' ', '[', ']'});
        }
        return propertyList;
    }

    //Extracts the properties from a string which contains a message
    std::optional<std::vector<std::string>> extractPropertiesMessage(const std::string& properties, ObjectID id) {
        /*
        std::string quote;
        auto lastQuotePos = [](const std::string& str) {
            for (int i = str.length() - 1; i >= 0; i--) {
                if (str[i] == '\"') {
                    return (size_t)i;
                }
            }
            return std::string::npos;
        };

        if (id == ObjectID::Message) {
            auto finalQuote = lastQuotePos(properties);
            quote = properties.substr(1, finalQuote - 1);

            auto lastPart = properties.substr(finalQuote + 1);
            auto lastProps = split(lastPart, ',');
            std::cout << "Properties: " << properties << "\n";
            std::cout << "Quote: " << quote << "\n";
            for (auto& p : lastProps) {
                std::cout << "Last prop: <" << p << ">\n";
            }
            std::cout << "\n\n";
        }
        else if (id == ObjectID::Portal) {
            auto gameNumBeginPos = [&properties]() {
                bool second = false;
                for (int i = properties.length() - 1; i >= 0; i--) {
                    if (properties[i] == '\"' && !second) {
                        second = true;
                    } 
                    else if (properties[i] == '\"') {
                        return (size_t)i;
                    } 
                }
                return std::string::npos;
            }();
            auto gameNumber = properties.substr(gameNumBeginPos + 1);
            gameNumber.pop_back();

            auto firstPart = properties.substr(0, gameNumBeginPos - 2);
            auto firstQuoteEnd = lastQuotePos(firstPart);
            auto quote = firstPart.substr(1, firstQuoteEnd - 1);
            auto props = split(firstPart, ',', true);

            return {quote, props.back(), gameNumber};
        }
        */
        auto numQuotes = std::count(properties.cbegin(), properties.cend(), '\"');
        if (id == ObjectID::Message && numQuotes != 2) {
            return {};
        }
        else if (id == ObjectID::Portal && numQuotes != 4) {
            return {};
        }
        auto props = split(properties, ',', true);

        auto removeOuterQuotes = [](std::string& s) {
            return s.substr(1, s.length() - 2);
        };
        if (id == ObjectID::Message) {
            removeOuterQuotes(props[0]);
        }
        else if (id == ObjectID::Portal) {
            removeOuterQuotes(props[0]);
            removeOuterQuotes(props[2]);
        }

        return props;
    }
}

std::optional<CYLevel> parseFile(const std::string& fileName) {
    CYLevel level;
    auto content = getFileContent(fileName);
    content.pop_back();

    auto metaDataEndLocation = findIgnoreQuotes(content, "Floor");
    if (metaDataEndLocation == std::string::npos) {
        metaDataEndLocation = findIgnoreQuotes(content, "floor");
    }
    if (metaDataEndLocation == std::string::npos) {
        errors.emplace_back(fileName, "Cannot find #Floor section");
        return {};
    } 

    auto metadata = content.substr(0, metaDataEndLocation);
    auto objects  = content.substr(metaDataEndLocation - 1);
    auto metaDataTokens = split(metadata, '#', true);

    //Extract metadata from the file
    level.name      = getMetadataAttribute("name",    metaDataTokens[1], true);
    level.numFloors = getMetadataAttribute("levels",  metaDataTokens[2], false);
    level.version   = getMetadataAttribute("version", metaDataTokens[3], false);
    level.creator   = getMetadataAttribute("creator", metaDataTokens[4], true); 

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
                removeFrom(name, {',', '#', ':', '[', '\"', ' '});
                std::transform(name.begin(), name.end(), name.begin(), ::tolower);
                tokens.emplace_back(std::move(name), objects.substr(begin, length + 1));
            }
        }
        else if (c == '\"') {
            isInsideString = !isInsideString;
        }
    }

    //Extraction of all the data objcts
    for (const auto& tokenPair : tokens) {
        const auto  objId   = stringToObjectID(tokenPair.first);
        const auto& data    = tokenPair.second;

        if (objId == ObjectID::Unknown) {
            continue;
        }
        
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
        switch(objId) {
            /**
             * Extract floors; which have a specific layout 
             */
            case ObjectID::Floor:{
                std::vector<CYFloor> floors;
                uint8_t floorNumber = 0;
                for (size_t i = 0; i < s.size() - 1; i += 8) {
                    CYFloor floor;
                    floor.vertexA = extractPosition(getMatchSection(s[i    ], d));
                    floor.vertexB = extractPosition(getMatchSection(s[i + 1], d));
                    floor.vertexC = extractPosition(getMatchSection(s[i + 2], d));
                    floor.vertexD = extractPosition(getMatchSection(s[i + 3], d));
                    auto properties = extractProperties(d.substr(s[i + 6].first, s[i + 6].second));
                    floor.topTexture = convertTexture(objId, properties[0]);
                    floor.bottomTexture = convertTexture(objId, properties[2]);
                    floor.isVisible = std::stoi(properties[1]);
                    floor.floor = ++floorNumber;
                    floors.push_back(std::move(floor));
                }
                level.floors = std::move(floors);
            }break;

            /**
             * Extract walls; which have a specific layout 
             */
            case ObjectID::Wall:{
                std::vector<CYWall> walls;
                for (size_t i = 0; i < s.size() - 1; i += 2) {
                    if (data.find("<Void>") != std::string::npos) {
                        errors.emplace_back(fileName, "Wall contains '<Void>'");
                        return {};
                    }
                    auto tokens = split(d.substr(s[i + 1].first, s[i + 1].second), ',');

                    int xOffset = std::stoi(tokens[0]);
                    int zOffset = std::stoi(tokens[1]);

                    int xBegin  = std::stoi(tokens[2]);
                    int zBegin  = std::stoi(tokens[3]);

                    auto props = extractProperties(d.substr(s[i].first, s[i].second));

                    CYWall wall;
                    wall.frontTexture = convertTexture(objId, props[0]);
                    wall.backTexture = convertTexture(objId, props[1]);
                    wall.beginPoint = {xBegin,              zBegin};
                    wall.endPoint   = {xBegin + xOffset,    zBegin + zOffset};
                    wall.properties = props;
                    wall.floor      = std::stoi(tokens.back());
                    wall.verifyPropertyCount();
                    wall.properties.erase(wall.properties.begin());
                    wall.properties.erase(wall.properties.begin());

                    walls.push_back(std::move(wall));
                }
                level.walls = std::move(walls);
            }break;

            /**
             * Extract all other objects types that do not have a special layout
             */
            default: {
                std::vector<CYObject> objects; 
                for (size_t i = 0; i < s.size() - 1; i += 3) {
                    auto fullData = getMatchSection(s[i + 2], d);

                    CYObject object;
                    switch (objId) {
                        case ObjectID::Message:{
                            auto p = extractPropertiesMessage(d.substr(s[i + 1].first, s[i + 1].second), objId);
                            if (!p) {
                                errors.emplace_back(fileName, "Message contains too many quotes");
                                return {};
                            }
                            object.properties = *p;
                            }break;
                        case ObjectID::Portal:{
                            auto p = extractPropertiesMessage(d.substr(s[i + 1].first, s[i + 1].second), objId);
                            if (!p) {
                                errors.emplace_back(fileName, "Portal contains too many quotes");
                                return {};
                            }
                            object.properties = *p;
                            }break;

                        //Some objects had a pointless first property
                        case ObjectID::Key:
                        case ObjectID::Teleport:
                            object.properties.push_back(extractProperties(d.substr(s[i + 1].first, s[i + 1].second))[1]);
                            break;

                        default: 
                            object.properties = extractProperties(d.substr(s[i + 1].first, s[i + 1].second));
                            break;
                    }

                    //Remove backmusic, weather, and theme, and instead use them as metadata for the level
                    switch(objId) {
                        case ObjectID::Music:
                            level.backmusic = std::stoi(object.properties[0]);
                            continue;
                        
                        case ObjectID::Weather:
                            level.weather = std::stoi(object.properties[0]);
                            continue;

                        case ObjectID::Theme:
                            level.theme = std::stoi(object.properties[0]);
                            continue;

                        default:
                            break;
                    }

                    object.position = extractPosition(getMatchSection(s[i], d));
                    object.floor = std::stoi(split(fullData, ',').back());
                    object.verifyPropertyCount(objId);

                    if (hasTexture(objId)) {
                        if (objId == ObjectID::Door) {
                            object.texture = convertTexture(objId, object.properties[2]);
                            object.properties.pop_back();
                        } else {
                            object.texture = convertTexture(objId, object.properties[1]);
                            object.properties.erase(object.properties.begin() + 1);
                        }
                        if (objId == ObjectID::DiaPlatform) {
                        }
                    }

                    objects.push_back(std::move(object));
                }
                switch (objId) {
                    case ObjectID::Music:
                    case ObjectID::Weather:
                    case ObjectID::Theme:
                        break;

                    default:
                        level.objects[(int)objId] = std::move(objects);
                        break;

                }
            }break;
        }
    }

    return level;
}

void printErrors() {
    std::cout   << "\n=======================================\n"
                << "Printing all error levels came across: \n";
    for (const auto& error : errors) {
        std::cout   << "File: " << error.name << "\n"
                    << "\tReason: " << error.reason << "\n\n";
    }
}