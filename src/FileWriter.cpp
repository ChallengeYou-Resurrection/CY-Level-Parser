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
        Message     = 6,

        Unknown     = 250,
    };

    void writeObjectTypeHeader(BinaryFileBuffer& buffer, ObjectID id, 
        size_t numObjects, const std::initializer_list<PType>& format) 
    {
        buffer << (u8)id << (u8)format.size() << (u32)numObjects;
        for (auto& property : format) {
            buffer << (u8)property;
        }
    }   

    void writeSingleObjectHeader(BinaryFileBuffer& buffer, const Position& position, uint8_t floor) {
        buffer << position << (u8)floor;
    }

    void writeObjectGroup(BinaryFileBuffer& buffer, const CYLevel& level, 
            ObjectID id, const std::initializer_list<PType>& format) 
    {
        if (level.numObjects(id)) {
            writeObjectTypeHeader(buffer, id, level.numObjects(id), format);
            for (const auto& obj : level.objects[(int)id]) {
                const auto& props = obj.properties;
                size_t index = 0;
                for (auto ptype : format) {
                    switch(ptype) {
                        case PType::Texture:
                            buffer << (u32)std::stoul(props[index]);
                            break;

                        case PType::Message:
                            buffer << props[index];
                            break;

                        default:
                            buffer << (u8)std::stoi(props[index]);
                            break;
                    }
                    index++;
                }
            }
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
    writeObjectGroup(bBuffer, level, ObjectID::Platform, 
        {PType::Size, PType::Texture, PType::QValue});
    

    //Portal		
    
    //Ramp		
    
    //Shield		
    
    //Slingshot	
    
    //Start		
    
    //Teleport	

    //TriPlatform 
    writeObjectGroup(bBuffer, level, ObjectID::TriPlatform, 
        {PType::Size, PType::Texture, PType::Direction, PType::QValue});
    
    //TriWall		


    //Final output
    std::ofstream outfile3(fileName + "_v2", std::ofstream::binary);
    outfile3.write(bBuffer.getBuffer(), bBuffer.bufferSize());
}