#include "Benchmark.h"
#include "FileParser.h"
#include "FileWriter.h"
#include "Utilities.h"

#include <filesystem>
#include <fstream>

#include "FileWriter.h"

namespace fs = std::filesystem;

void testLocal(const std::string& name)
{
    auto level = parseFile("games/" + name);
    if (level) {
        writeLevelJson(name, *level);
    }
}

int main()
{
    // benchmark::Benchmark<25> bench("Time: ", &testLocal,
    //                               "158209.The Mouse Returns.Haakson.cy");
    // bench.outputTimes();

    benchmark::Timer<> timer;
    testLocal("158209.The Mouse Returns.Haakson.cy");
    std::cout << "Time: " << timer.getTime() << "ms\n";
    return 0;
    for (const auto& path : fs::directory_iterator("games/")) {
        testLocal(path.path().filename().string());
    }
    std::cout << "TOTAL TIME: " << timer.getTime() << "ms" << std::endl;
    printErrors();
    return 0;
}
/*
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
*/