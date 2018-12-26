#include "FileParser.h"
#include "Utilities.h"
#include "Benchmark.h"
#include "FileWriter.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

#define USE_SAMPLE_GAMES 
#ifdef USE_SAMPLE_GAMES
    #define SINGLE_GAME
    #ifdef SINGLE_GAME
        #define DIR "../../single"
        #define OUT "../../single_out/"
    #else
        #define DIR "../../sample"
        #define OUT "../../sample_out/"
    #endif
#else 
    #define DIR "../../Games"
    #define OUT "../../out/"
#endif

//This is tempory for testing, will be changed later
void writeLevel(const std::string& name, const CYLevel& level) {
    std::ofstream outfile(OUT + name + ".out");
    outfile << "Name:    "  << level.name           << '\n' 
            << "Author:  "  << level.creator        << '\n' 
            << "Version: "  << level.version        << '\n'
            << "Floors:  "  << level.numFloors      << '\n'
            << "Theme:   "  << level.theme          << '\n'
            << "Music:   "  << level.backmusic      << '\n'
            << "Weather: "  << level.weather        << '\n';

    outfile << "Floors\n";
    for (const auto& floor : level.floors) {
        outfile << "\tVertex 1:   " << floor.vertexA.x << " " << floor.vertexA.z << '\n'
                << "\tVertex 2:   " << floor.vertexB.x << " " << floor.vertexB.z << '\n'
                << "\tVertex 3:   " << floor.vertexC.x << " " << floor.vertexC.z << '\n'
                << "\tVertex 4:   " << floor.vertexD.x << " " << floor.vertexD.z << '\n'
                << "\tFloor:      " << (int)floor.floor << '\n'
                << "\tProperties: ";
        for (const auto& p : floor.properties) {
            outfile << p << ' ';
        }
        outfile << "\n\n";
    }

    outfile << "Walls\n";
    for (const auto& wall : level.walls) {
        outfile << "\tBegin:      " << wall.beginPoint.x    << " " << wall.beginPoint.z << '\n'
                << "\tEnd:        " << wall.endPoint.x      << " " << wall.endPoint.z   << '\n'
                << "\tFloor:      " << (int)wall.floor      << '\n'
                << "\tProperties: ";
        for (const auto& p : wall.properties) {
            outfile << p << ' ';
        }
        outfile << "\n\n";               
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
            outfile << "\n\n";
        }
    }
}

std::optional<CYLevel> readFile(const std::string& name) {
    return parseFile(name.c_str());
}

void testLocal(const std::string& name) {
    std::string path = "../../Games/" + name;
    auto level = readFile(path);
    if (level) {
        std::cout << "Writing level!\n";
        writeLevel(name, *level);
    }   
}

auto getGamesDirectoryItr() {
    return fs::directory_iterator(DIR);
}

int main() {
#ifndef USE_SAMPLE_GAMES
    benchmark::Timer<> timer;
    const int percentIncrement = std::distance(getGamesDirectoryItr(), fs::directory_iterator{}) / 100;
    int count = 0;
    int progress = 0;
    std::cout << "Total files: " << percentIncrement * 100 << std::endl;
    timer.reset();
#endif
    for (const auto& path : getGamesDirectoryItr()) {
#ifndef USE_SAMPLE_GAMES
        if (++count % percentIncrement == 0) {
            printf("Progress: %d%% [%d out of %d games converted] ", ++progress, count, percentIncrement * 100);
            printf("[Time: %fms]\n", timer.getTime());
            timer.reset();
        }
#endif

        const std::string name = path.path().filename().string();
        std::cout << "Doing: " << name << "'\n";
        auto level = readFile(path.path().c_str());
        if (level) {
#ifdef SINGLE_GAME
#define TIMES 1
            benchmark::Benchmark<TIMES>(std::string(name + ": Text  ").c_str(), 
                &writeLevel, name, *level).outputTimes();
            benchmark::Benchmark<TIMES>(std::string(name + ": Binary").c_str(), 
                &writeLevelBinary, *level, OUT + name + ".cyb").outputTimes();
            std::cout << "\n\n\n\n\n\n\n";
#else 
            writeLevelBinary(*level, OUT + name + ".cyb");
            writeLevel(name, *level);
#endif
        }   
    }
    printErrors();
    return 0;
}
