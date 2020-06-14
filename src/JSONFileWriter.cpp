#include "CYObject.h"
#include "FileWriter.h"
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <lz4/lz4.h>

namespace {
    enum class WallShape {
        Rectangle = 0,
        TriWall = 1,
        InvertedTriWall = 2,
    };

    float cyPos(int pos)
    {
        return static_cast<float>(static_cast<int>((float)(pos / 5.0f) * 10.f)) / 10.f;
    }

    struct JsonWriter {
        nlohmann::json root;

        nlohmann::json walls = nlohmann::json::array();
        nlohmann::json floors = nlohmann::json::array();

        JsonWriter()
        {
            root["Objects"] = nlohmann::json::object();
        }

        const nlohmann::json& compile()
        {
            root["Objects"]["Wall"] = walls;
            root["Objects"]["Level Ground"] = floors;

            return root;
        }

        void addWall(const Position& begin, const Position& end, int lvl,
                     const Texture& texture1, const Texture& texture2, float h1, float h2,
                     WallShape shape)
        {
            nlohmann::json jsonWall;
            jsonWall["Texture"] = texture1.id;
            jsonWall["Colour"] = texture1.colour;
            jsonWall["Texture2"] = texture2.id;
            jsonWall["Colour2"] = texture2.colour;

            jsonWall["Pos"] = {cyPos(begin.x), cyPos(begin.z)};
            jsonWall["End"] = {cyPos(end.x), cyPos(end.z)};

            jsonWall["Lvl"] = lvl;

            jsonWall["Height"] = {h1, h2};
            jsonWall["Shape"] = (int)shape;

            walls.push_back(jsonWall);
        }
    };
} // namespace

void writeLevelJson(const std::string& name, const CYLevel& level)
{
    JsonWriter writer;
    writer.root["Name"] = level.name;
    writer.root["Author"] = level.creator;
    writer.root["Version"] = level.version;
    writer.root["Levels"] = level.numFloors;

    // json["theme"] = level.theme;
    // json["music"] = level.backmusic;
    // json["weather"] = level.weather;

    //
    //  Floors
    //
    /*
    for (const auto& floor : level.floors) {
        nlohmann::json jsonFloor;
        jsonFloor["texture"] = nlohmann::json::array(
            {floor.topTexture->asString(), floor.bottomTexture->asString()});

        jsonFloor["position"] = nlohmann::json::array(
            {cyPos(floor.vertexA.x), cyPos(floor.vertexA.z), cyPos(floor.vertexB.x),
             cyPos(floor.vertexB.z), cyPos(floor.vertexC.x), cyPos(floor.vertexC.z),
             cyPos(floor.vertexD.x), cyPos(floor.vertexD.z)});

        jsonFloor["Lvl"] = floor.floor;
        jsonFloor["properties"] = floor.isVisible;

        floors.push_back(jsonFloor);
    }
    json["Objects"]["Level Ground"] = floors;
    */

    //
    //  Walls
    //
    for (const auto& wall : level.walls) {
        // TO DO, do somethinh with the only property a wall has for the wall height
        writer.addWall(wall.beginPoint, wall.endPoint, wall.floor, wall.frontTexture,
                       wall.backTexture, 0, 4, WallShape::Rectangle);
    }

    //
    //  Objects
    //
    
    for (size_t i = 0; i < level.objects.size(); i++) {
        nlohmann::json objects = nlohmann::json::array();

        switch ((ObjectID)i) {
            case ObjectID::TriWall: {
                for (const auto& triwall : level.objects[i]) {
                    Position begin;
                    Position end;

                    bool flipped =
                        triwall
                            .properties[]

                    writer.addWall(begin, end, triwall.floor, triwall.texture,
                                   triwall.texture, 0, 4, WallShape::TriWall);
                }
            } break;

            default:
                break;
        }


                                    /*
        for (const auto& obj : level.objects[i]) {

            nlohmann::json jsonObj;
            jsonObj["Lvl"] = obj.floor;
            jsonObj["position"] =
                nlohmann::json::array({cyPos(obj.position.x), cyPos(obj.position.z)});

            jsonObj["properties"] = nlohmann::json::array();
            for (const auto& p : obj.properties) {
                jsonObj["properties"].push_back(p);
            }
            if (hasTexture((ObjectID)i)) {
                jsonObj["texture"] = obj.texture->asString();
            }
            objects.push_back(jsonObj);
        }
        */
        json["lvl"][objectIdToString((ObjectID)i)] = objects;
    }

    auto& json = writer.compile();

    auto data = json.dump(2);
    std::ofstream jsonFile("out/" + name + ".json");
    jsonFile << data;

    const char* const source = data.c_str();
    int size = (int)(std::strlen(source) + 1);
    int maxSize = LZ4_compressBound(size);
    char* compressed = new char[maxSize];
    if (!compressed) {
        std::cout << "Failed to allocate...";
        return;
    }
    int compressedSize = LZ4_compress_default(source, compressed, size, maxSize);
    if (compressedSize <= 0) {
        std::cout << "Failed to compress...";
        return;
    }

    std::ofstream bin(OUT + name + ".json.lz4", std::ios::binary);
    bin.write(compressed, compressedSize);
    delete[] compressed;
}