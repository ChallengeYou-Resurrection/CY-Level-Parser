#include "Benchmark.h"
#include "FileParser.h"
#include "FileWriter.h"
#include "Utilities.h"

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

const char* OUT = "out/";

#include <json.hpp>
#include <lz4/lz4.h>

/**
    Writes a CY Level as JSON

    t = texture
    p = position
    f = floor
    pr = properties
*/
void writeLevelJson(const std::string& name, const CYLevel& level)
{
    nlohmann::json json;
    json["name"] = level.name;
    json["author"] = level.creator;
    json["version"] = level.version;
    json["floors"] = level.numFloors;
    json["theme"] = level.theme;
    json["music"] = level.backmusic;
    json["weather"] = level.weather;
    json["lvl"] = nlohmann::json::object();

    //
    //  Floors
    //
    nlohmann::json floors = nlohmann::json::array();
    for (const auto& floor : level.floors) {
        nlohmann::json jsonFloor;
        jsonFloor["texture"] = nlohmann::json::array(
            {floor.topTexture->asString(), floor.bottomTexture->asString()});

        jsonFloor["position"] = nlohmann::json::array(
            {floor.vertexA.x, floor.vertexA.z, floor.vertexB.x, floor.vertexB.z,
             floor.vertexC.x, floor.vertexC.z, floor.vertexD.x, floor.vertexD.z});

        jsonFloor["floor"] = floor.floor;
        jsonFloor["properties"] = floor.isVisible;

        floors.push_back(jsonFloor);
    }
    json["lvl"]["floors"] = floors;

    //
    //  Walls
    //
    nlohmann::json walls = nlohmann::json::array();
    for (const auto& wall : level.walls) {
        nlohmann::json jsonWall;
        jsonWall["texture"] = nlohmann::json::array(
            {wall.frontTexture->asString(), wall.backTexture->asString()});

        jsonWall["position"] = nlohmann::json::array(
            {wall.beginPoint.x, wall.beginPoint.z, wall.endPoint.x, wall.endPoint.z});

        jsonWall["floor"] = wall.floor;
        jsonWall["properties"] = nlohmann::json::array();
        for (const auto& p : wall.properties) {
            jsonWall["properties"].push_back(p);
        }
        walls.push_back(jsonWall);
    }
    json["lvl"]["walls"] = walls;

    //
    //  Objects
    //
    for (size_t i = 0; i < level.objects.size(); i++) {
        nlohmann::json objects = nlohmann::json::array();
        for (const auto& obj : level.objects[i]) {
            nlohmann::json jsonObj;
            jsonObj["floor"] = obj.floor;
            jsonObj["position"] = nlohmann::json::array({obj.position.x, obj.position.z});

            jsonObj["properties"] = nlohmann::json::array();
            for (const auto& p : obj.properties) {
                jsonObj["properties"].push_back(p);
            }
            if (hasTexture((ObjectID)i)) {
                jsonObj["texture"] = obj.texture->asString();
            }
            objects.push_back(jsonObj);
        }
        json["lvl"][objectIdToString((ObjectID)i)] = objects;
    }

    auto data = json.dump();

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

// This is tempory for testing, will be changed later
void writeLevel(const std::string& name, const CYLevel& level)
{
    std::ofstream outfile(OUT + name + ".out");
    outfile << "Name:    " << level.name << '\n'
            << "Author:  " << level.creator << '\n'
            << "Version: " << level.version << '\n'
            << "Floors:  " << level.numFloors << '\n'
            << "Theme:   " << level.theme << '\n'
            << "Music:   " << level.backmusic << '\n'
            << "Weather: " << level.weather << '\n';

    outfile << "Floors\n";
    for (const auto& floor : level.floors) {
        outfile << "\tVertex 1:   " << floor.vertexA.x << " " << floor.vertexA.z << '\n'
                << "\tVertex 2:   " << floor.vertexB.x << " " << floor.vertexB.z << '\n'
                << "\tVertex 3:   " << floor.vertexC.x << " " << floor.vertexC.z << '\n'
                << "\tVertex 4:   " << floor.vertexD.x << " " << floor.vertexD.z << '\n'
                << "\tFloor:      " << (int)floor.floor << '\n'
                << "\tTex Top:    " << floor.topTexture->asString() << '\n'
                << "\tTex Bottom: " << floor.topTexture->asString() << '\n'
                << "\tVisible: " << (int)floor.isVisible << '\n';
        outfile << "\n";
    }

    outfile << "Walls\n";
    for (const auto& wall : level.walls) {
        outfile << "\tBegin:      " << wall.beginPoint.x << " " << wall.beginPoint.z
                << '\n'
                << "\tEnd:        " << wall.endPoint.x << " " << wall.endPoint.z << '\n'
                << "\tFloor:      " << (int)wall.floor << '\n'
                << "\tTex Front:  " << wall.frontTexture->asString() << '\n'
                << "\tTex Back:   " << wall.backTexture->asString() << '\n'
                << "\tProperties: ";
        for (const auto& p : wall.properties) {
            outfile << p << ' ';
        }
        outfile << "\n";
    }

    for (size_t i = 0; i < level.objects.size(); i++) {
        outfile << "Object ID: " << i << '\n';
        for (const auto& obj : level.objects[i]) {
            outfile << "\tPosition:   " << obj.position.x << " " << obj.position.z << '\n'
                    << "\tFloor:      " << (int)obj.floor << '\n'
                    << "\tProperties: ";
            for (const auto& p : obj.properties) {
                if (i == (int)ObjectID::Message) {
                    outfile << "\n\tMessage Prop: " << p;
                }
                else {
                    outfile << p << ' ';
                }
            }
            if (hasTexture((ObjectID)i)) {
                outfile << "Tex: " << obj.texture->asString() << '\n';
            }
            outfile << "\n";
        }
    }
}

void testLocal(const std::string& name)
{
    auto level = parseFile("games/" + name);
    if (level) {
        writeLevelJson(name, *level);
    }
}

int main()
{
    benchmark::Benchmark<25> bench("Time: ", &testLocal,
                                   "158209.The Mouse Returns.Haakson.cy");
    bench.outputTimes();
    //testLocal("158209.The Mouse Returns.Haakson.cy");
    return 0;
    benchmark::Timer<> timer;
    for (const auto& path : fs::directory_iterator("games/")) {
        testLocal(path.path().filename().string());
    }
    std::cout << "TOTAL TIME: " << timer.getTime() << "ms" << std::endl;

    /*
#ifndef USE_SAMPLE_GAMES
    benchmark::Timer<> timer;
    const int percentIncrement = std::distance(getGamesDirectoryItr(),
fs::directory_iterator{}) / 100; int count = 0; int progress = 0; std::cout << "Total
files: " << percentIncrement * 100 << std::endl; timer.reset(); #endif for (const auto&
path : getGamesDirectoryItr()) { #ifndef USE_SAMPLE_GAMES if (++count % percentIncrement
== 0) { printf("Progress: %d%% [%d out of %d games converted] ", ++progress, count,
percentIncrement * 100); printf("[Time: %fms]\n", timer.getTime()); timer.reset();
        }
#endif
        const std::string name = path.path().filename().string();
        auto level = readFile(path.path().string());

        if (level) {
#ifdef SINGLE_GAME
#define TIMES 1
            //benchmark::Benchmark<TIMES>(std::string(name + ": Text  ").c_str(),
            //    &writeLevel, name, *level).outputTimes();
            //benchmark::Benchmark<TIMES>(std::string(name + ": Binary").c_str(),
            //    &writeLevelBinary, *level, OUT + name + ".cyb").outputTimes();
            //std::cout << "\n\n\n\n\n\n\n";
#else
            //writeLevelBinary(*level, OUT + name + ".cyb");
            writeLevel(name, *level);
#endif
        }
    }
    */
    printErrors();
    return 0;
}
