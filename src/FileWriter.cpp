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
        Texture     = 0, //Texture
        QValue      = 1, //The "height" of objects, eg walls and platforms 
        IsHidden    = 2, //For floors, whether or not it is hidden or not
        Size        = 3, //Resizable objects such as all platforms, and pillars
        Direction   = 4, //Directional objects such as triplats, boards, ramps etc
        Message     = 5, //For objects that have a string (eg Portal and Board)
        Flip        = 6, //For objects that have a binary option eg pillars stright or dia, or triwalls flip
        FuelType    = 7, //For JetPacks; Whether the jetpack needs fuel or nah
        WinCond     = 8, //Win condition, such as portals and finish marker
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
    writeObjectGroup(bBuffer, level, ObjectID::DiaPlatform, 
        {PType::Size, PType::Texture, PType::QValue});

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
    writeObjectGroup(bBuffer, level, ObjectID::Platform, 
        {PType::Flip, PType::Texture, PType::Size, PType::QValue});
    
    //Platform	
    writeObjectGroup(bBuffer, level, ObjectID::Platform, 
        {PType::Size, PType::Texture, PType::QValue});
    

    //Portal		
    
    //Ramp	
    writeObjectGroup(bBuffer, level, ObjectID::Ramp, 
        {PType::Direction, PType::Texture});	
    
    //Shield		
    
    //Slingshot	
    
    //Start		
    
    //Teleport	

    //TriPlatform 
    writeObjectGroup(bBuffer, level, ObjectID::TriPlatform, 
        {PType::Size, PType::Texture, PType::Direction, PType::QValue});
    
    //TriWall	
    writeObjectGroup(bBuffer, level, ObjectID::TriWall, 
        {PType::Flip, PType::Texture, PType::Direction});	


    //Final output
    std::ofstream outfile3(fileName + "_v2", std::ofstream::binary);
    outfile3.write(bBuffer.getBuffer(), bBuffer.bufferSize());
}