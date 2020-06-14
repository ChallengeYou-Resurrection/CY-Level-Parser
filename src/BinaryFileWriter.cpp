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
        Message     = 5, //String (eg Portal and Board)
        Flip        = 6, //binary geometry option eg pillars stright or dia, or triwalls flip
        FuelType    = 7, //For JetPacks; Whether the jetpack needs fuel or nah
        WinCond     = 8, //Win condition, such as portals and finish marker
        Amount      = 9, //Objects such as crumbs, sheilds, and fuel allow you to choose amount
        TeleID      = 10,   //Teleports connect to eachother
        KeyID       = 11,   //Key to doors
        Speed       = 12,   //Icemen speed
        Strength    = 13,   //Icemen strength
        ChaserStyle = 14,   //Chasers can be saucer, pumpkin, or ghost
        DiamondType = 14,   //Diamonds have different types
    };  

    //Writes header info about an object
    void writeObjectTypeHeader(BinaryFileBuffer& buffer, ObjectID id, 
        size_t numObjects, const std::initializer_list<PType>& format) 
    {
        buffer << (u8)id << (u8)format.size() << (u32)numObjects;
        for (auto& property : format) {
            buffer << (u8)property;
        }
    }   

    //Writes the position and floor of a single object
    void writeSingleObjectHeader(BinaryFileBuffer& buffer, const Position& position, uint8_t floor) {
        buffer << position << (u8)floor;
    }

    //Write an object group for when the properties are ALL u8 (this is just for a slight optimization)
    void writeAllU8Group(BinaryFileBuffer& buffer, const CYLevel& level, 
            ObjectID id, const std::initializer_list<PType>& format)
    {
        if (level.numObjects(id)) {
            writeObjectTypeHeader(buffer, id, level.numObjects(id), format);
            for (const auto& obj : level.objects[(int)id]) {
                writeSingleObjectHeader(buffer, obj.position, obj.floor);
                const auto& props = obj.properties;
                for (size_t i = 0; i < format.size(); i++) { 
                    buffer << (u8)std::stoi(props[i]);
                }
            }
        }
    }

    //Write an object group for mixture of property types
    void writeObjectGroup(BinaryFileBuffer& buffer, const CYLevel& level, 
            ObjectID id, const std::initializer_list<PType>& format) 
    {
        if (level.numObjects(id)) {
            writeObjectTypeHeader(buffer, id, level.numObjects(id), format);
            for (const auto& obj : level.objects[(int)id]) {
                writeSingleObjectHeader(buffer, obj.position, obj.floor);
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
    writeAllU8Group(bBuffer, level, ObjectID::Chaser, 
        {PType::Speed, PType::ChaserStyle});
    //Crumbs	
    writeAllU8Group(bBuffer, level, ObjectID::Crumbs, 
        {PType::Amount});
    
    //DiaPlatform	
    writeObjectGroup(bBuffer, level, ObjectID::DiaPlatform, 
        {PType::Size, PType::Texture, PType::QValue});

    //Diamond		
    writeAllU8Group(bBuffer, level, ObjectID::Diamond, 
        {PType::DiamondType, PType::QValue});

    //Door	
    writeObjectGroup(bBuffer, level, ObjectID::Door, 
        {PType::Flip, PType::KeyID, PType::Texture});	
    
    //Finish		
    writeAllU8Group(bBuffer, level, ObjectID::Finish, 
        {PType::WinCond});	
    
    //Fuel	
    writeAllU8Group(bBuffer, level, ObjectID::Fuel, 
        {PType::Amount});	
    
    //Hole		
    writeAllU8Group(bBuffer, level, ObjectID::Hole, 
        {PType::Size});
    
    //Iceman
    writeAllU8Group(bBuffer, level, ObjectID::Iceman, 
        {PType::Speed, PType::Strength});	
    	
    
    //JetPack		
    writeAllU8Group(bBuffer, level, ObjectID::JetPack, 
        {PType::FuelType});	
    
    //Key		
    writeAllU8Group(bBuffer, level, ObjectID::Key, 
        {PType::KeyID});		    
    
    //Ladder		
    writeAllU8Group(bBuffer, level, ObjectID::Ladder, 
        {PType::Direction});
    
    //Message		
    writeObjectGroup(bBuffer, level, ObjectID::Message, 
        {PType::Message, PType::Direction, PType::QValue});

    //Pillar
    writeObjectGroup(bBuffer, level, ObjectID::Pillar, 
        {PType::Flip, PType::Texture, PType::Size, PType::QValue});
    
    //Platform	
    writeObjectGroup(bBuffer, level, ObjectID::Platform, 
        {PType::Size, PType::Texture, PType::QValue});
    
    //Portal	
    writeObjectGroup(bBuffer, level, ObjectID::Portal, 
        {PType::Message, PType::WinCond, PType::Message});
    
    //Ramp	
    writeObjectGroup(bBuffer, level, ObjectID::Ramp, 
        {PType::Direction, PType::Texture});	
    
    //Shield	
    writeAllU8Group(bBuffer, level, ObjectID::Shield, 
        {PType::Amount});		
    
    //Slingshot	
    writeAllU8Group(bBuffer, level, ObjectID::Slingshot, 
        {PType::QValue});	
    
    //Start		
    writeAllU8Group(bBuffer, level, ObjectID::Start, 
        {PType::Direction});	
    
    //Teleport	
    writeAllU8Group(bBuffer, level, ObjectID::Teleport, 
        {PType::TeleID});	

    //TriPlatform 
    writeObjectGroup(bBuffer, level, ObjectID::TriPlatform, 
        {PType::Size, PType::Texture, PType::Direction, PType::QValue});
    
    //TriWall	
    writeObjectGroup(bBuffer, level, ObjectID::TriWall, 
        {PType::Flip, PType::Texture, PType::Direction});	


    //Final output and create a filename to output as
    auto pathidx = [&fileName]() {
        for (size_t i = 0; i < fileName.size(); i++) {
            if (std::isdigit(fileName[i])) {
                return i;
            }
        }
        return std::string::npos;
    }();
    auto path = fileName.substr(0, pathidx);
    auto gameNumber = fileName.substr(pathidx);
    gameNumber = gameNumber.substr(0, gameNumber.find('.'));
    auto outputName = path + level.name + "[" + gameNumber + "].cyb";

    std::ofstream outfile3(outputName, std::ofstream::binary);
    outfile3.write(bBuffer.getBuffer(), bBuffer.bufferSize());
}