#include "FileWriter.h"

#include <fstream>
#include <iostream>

#include "CYObject.h"

namespace {
    enum class PType : uint8_t {
        Position    = 0,
        Texture     = 1,
        Q_Value     = 2,
        Floor       = 3,
    };

    template<typename WriteType, typename OrigType>
    void writeNumber(std::ofstream& outFile, OrigType n) {
        WriteType converted = static_cast<WriteType>(n);
        outFile.write(reinterpret_cast<const char*>(&converted), sizeof(converted));
    }

    void writeString(std::ofstream& outFile, const std::string& string) {
        uint8_t size = (uint8_t)string.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        outFile.write(string.c_str(), string.size());
    }

    void writeObjectTypeHeader(std::ofstream& outFile, const std::initializer_list<PType>& format, ObjectID id, size_t numObjects) {
        writeNumber<uint8_t>(outFile, id);
        writeNumber<uint8_t>(outFile, format.size());//format.size(), outFile);
        for (auto p : format) {
            writeNumber<uint8_t>(outFile, p);
        }
        std::cout << "Number of objects: " << numObjects << std::endl;
        writeNumber<uint32_t>(outFile, numObjects);
    }

    void writePosition(std::ofstream& outFile, const Position& position) {
        auto x = position.x;
        auto z = position.z;

        writeNumber<int16_t>(outFile, x);
        writeNumber<int16_t>(outFile, z);
    }

    void writeSingleObjectHeader(std::ofstream& outFile, const Position& position, uint8_t floor) {
        writePosition(outFile, position);
        writeNumber<uint8_t>(outFile, floor);
    }

    void writeWalls(std::ofstream& outFile, const std::vector<CYWall>& walls) {
        writeObjectTypeHeader(
            outFile, 
            {PType::Position, PType::Position, PType::Texture, PType::Texture, PType::Floor, PType::Q_Value}, 
            ObjectID::Wall, walls.size());

        for (const auto& wall : walls) {
            writePosition(outFile, wall.beginPoint);
            writePosition(outFile, wall.endPoint);
            writeNumber<uint32_t>(outFile, std::stoi(wall.properties[0]));
            writeNumber<uint32_t>(outFile, std::stoi(wall.properties[1]));
            writeNumber<uint8_t>(outFile, wall.floor);
            writeNumber<uint8_t>(outFile, std::stoi(wall.properties[2]));
        }

    }
}


void writeLevelBinary(const CYLevel& level, const std::string& fileName) {
    std::ofstream outfile(fileName, std::ofstream::binary);

    writeString(outfile, level.name);
    writeString(outfile, level.creator);
    writeNumber<uint8_t>(outfile, std::stoi(level.numFloors));
    writeNumber<uint8_t>(outfile, level.backmusic);
    writeNumber<uint8_t>(outfile, level.theme);
    writeNumber<uint8_t>(outfile, level.weather);

    writeWalls(outfile, level.walls);
}