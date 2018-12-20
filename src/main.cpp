#include "FileParser.h"
#include "Benchmark.h"

#include <fstream>

int main(int argc, char** argv) {
    auto level = parseFile("The Mouse Returns.cy");

    std::ofstream outfile("out.cy");
    outfile << "Name:    "  << level.name           << '\n' 
            << "Author:  "  << level.creator        << '\n' 
            << "Version: "  << level.version        << '\n'
            << "Floors:  "  << level.numFloors      << '\n';
    for (const auto& cyObject : level.objects) {
        //outfile << cyObject.first << '\n';
        for (const auto& obj : cyObject.second) {
            outfile << "\tPosition:   " << obj.position.x << " " << obj.position.z << '\n'
                    << "\tProperties: " << obj.properties << '\n'
                    << "\tFloor:      " << (int)obj.floor << '\n';
        }
    }
}
