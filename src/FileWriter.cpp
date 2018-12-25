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
        Texture     = 0,
        QValue      = 1,
        Floor       = 2,
        IsHidden    = 3,
        Size        = 4,
        Direction   = 5,

        Unknown     = 250,
    };

    void writeObjectTypeHeader(BinaryFileBuffer& buffer, ObjectID id, size_t numObjects, const std::initializer_list<PType>& format) {
        buffer << (u8)id << (u8)format.size() << (u32)numObjects;
        for (auto& property : format) {
            buffer << (u8)property;
        }
    }   
}

void writeLevelBinary(const CYLevel& level, const std::string& fileName) {
    BinaryFileBuffer bBuffer;
    bBuffer << (u8)1 
            << level.name << level.creator 
            << (u8)std::stoi(level.numFloors)
            << (u8)level.backmusic << (u8)level.theme << (u8)level.weather;

    //Walls
    writeObjectTypeHeader(bBuffer, ObjectID::Wall, level.walls.size(), 
        {PType::Texture, PType::Texture, PType::QValue});
    for (const auto& wall : level.walls) {
        bBuffer << wall;
    } 

    //Floors
    writeObjectTypeHeader(bBuffer, ObjectID::Floor, level.floors.size(), 
        {PType::Texture, PType::Texture, PType::IsHidden});
    for (const auto& floor : level.floors) {
        bBuffer << floor;
    } 

    //Chaser		
    
    //Crumbs		
    
    //DiaPlatform	
    
    //Diamond		
    
    //Door	
    
    //Finish		
    
    //Fuel		
    
    //Hole		
    
    //Iceman		
    
    //JetPack		
    
    //Key		    
    
    //Ladder		
    
    //Message		
    
    //Pillar		
    
    //Platform	
    
    //Portal		
    
    //Ramp		
    
    //Shield		
    
    //Slingshot	
    
    //Start		
    
    //Teleport	
    
    //TriPlatform 
    
    //TriWall		
   //// writeObjectTypeHeader(bBuffer, ObjectID::TriPlatform, level.numObjects(ObjectID::TriPlatform), 
    //    {PType::Size, PType::Texture, PType::Direction});

    //Final output
    std::ofstream outfile3(fileName + "_v2", std::ofstream::binary);
    outfile3.write(bBuffer.getBuffer(), bBuffer.bufferSize());
}