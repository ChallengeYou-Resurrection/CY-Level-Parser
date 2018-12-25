#include "FileWriter.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "CYObject.h"
#include "BinaryFile.h"
#include "Benchmark.h"

namespace {
    enum class PType : uint8_t {
        Position    = 0,
        Texture     = 1,
        QValue      = 2,
        Floor       = 3,
        IsHidden    = 4,

        Unknown     = 250,
    };

    void writeObjectTypeHeader(BinaryFileBuffer& buffer, ObjectID id, size_t numObjects, const std::initializer_list<PType>& format) {
        buffer << (u8)id << (u8)format.size() << (u32)numObjects;
        for (auto& property : format) {
            buffer << (u8)property;
        }
    }

    void writeSingleObjectHeader(BinaryFileBuffer& buffer, const Position& position, uint8_t floor) {
        buffer << position << (u8)floor;
    }
    
}

void writeLevelBinary(const CYLevel& level, const std::string& fileName) {
    BinaryFileBuffer bBuffer;
    bBuffer << (u8)1 
            << level.name << level.creator 
            << (u8)std::stoi(level.numFloors)
            << (u8)level.backmusic << (u8)level.theme << (u8)level.weather;

    //WALLS
    writeObjectTypeHeader(bBuffer, ObjectID::Wall, level.walls.size(), 
        {PType::Texture, PType::Texture, PType::QValue});
    for (const auto& wall : level.walls) {
        bBuffer << wall;
    } 
            
    //Final output
    std::ofstream outfile3(fileName + "_v2", std::ofstream::binary);
    outfile3.write(bBuffer.getBuffer(), bBuffer.bufferSize());
}