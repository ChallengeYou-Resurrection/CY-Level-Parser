#include "FileParser.h"
#include "Utilities.h"
#include "Benchmark.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

//This is tempory for testing, will be changed later
void writeLevel(const std::string& name, const CYLevel& level) {
    std::ofstream outfile("../../sample_out/" + name + ".out");
    outfile << "Name:    "  << level.name           << '\n' 
            << "Author:  "  << level.creator        << '\n' 
            << "Version: "  << level.version        << '\n'
            << "Floors:  "  << level.numFloors      << '\n';

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
                << "\tProperties: " << wall.properties      << '\n'
                << "\tFloor:      " << (int)wall.floor      << "\n\n";
    }
    
    for (const auto& cyObject : level.objects) {
        outfile << cyObject.first << '\n';
        for (const auto& obj : cyObject.second) {
            outfile << "\tPosition:   " << obj.position.x << " " << obj.position.z << '\n'
                    << "\tFloor:      " << (int)obj.floor << '\n'
                    << "\tProperties: ";
            for (const auto& p : obj.properties) {
                outfile << p << ' ';
            }
            outfile << "\n\n";
        }
    }
}

std::optional<CYLevel> readFile(const std::string& name) {
    std::cout << "Parsing: " << name << '\n';
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

int main() {
    auto itr = fs::directory_iterator("../../sample");
    for (const auto& path : itr) {
        try {
            const std::string name = path.path().filename().string();
            auto level = readFile(path.path().c_str());
            if (level) {
                writeLevel(name, *level);
            }   
        }
        catch (std::exception& e) {
            std::cout << "Cannot parse: " << path.path().string() << '\n';
            std::cout << e.what() << '\n';
            return 0;
        }
    }
    printErrors();
    return 0;
}
