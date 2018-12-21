#include "FileParser.h"
#include "Benchmark.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void writeLevel(const std::string& name, const CYLevel& level) {
    std::ofstream outfile("../../out/" + name + ".out");
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
                << "\tProperties: " << floor.properties << '\n'
                << "\tFloors:     " << (int)floor.floor << "\n\n";
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
                    << "\tProperties: " << obj.properties << '\n'
                    << "\tFloor:      " << (int)obj.floor << "\n\n";
        }
    }
}

int main() {

    auto itr = fs::directory_iterator("../../Games");
    for (const auto& path : itr) {
        try {
            const std::string name = path.path().filename().string();
            std::cout << "Trying: " << name << '\n';
            auto level = parseFile(path.path().c_str());
            writeLevel(name, level);
            std::cout << "Sucess\n\n";
        }
        catch (std::exception& e) {
            std::cout << "Cannot parse: " << path.path().string() << '\n';
        }
    }
    return 0;
}
