#include "CYObject.h"

#include <iostream>
#include <unordered_map>

namespace {
    void addBackProp(std::vector<std::string>& propList)
    {
        propList.push_back("1");
    }

    void addFrontProp(std::vector<std::string>& propList)
    {
        propList.insert(propList.begin(), "1");
    }
} // namespace

void CYObject::verifyPropertyCount(ObjectID id)
{
    int propCount = properties.size();

    switch (id) {
        case ObjectID::Platform:
            switch (propCount) {
                case 1:
                    addFrontProp(properties);
                case 2: // Fallthrough is intentional
                    addBackProp(properties);
            }
            break;

        case ObjectID::Finish:
            if (propCount == 2) {
                properties.erase(properties.begin());
            }
            break;
        case ObjectID::Diamond:
        case ObjectID::Iceman:
        case ObjectID::Ramp:
            if (propCount == 1) {
                addBackProp(properties);
            }
            break;

        case ObjectID::Message:
        case ObjectID::DiaPlatform:
        case ObjectID::Door:
            if (propCount == 2) {
                addBackProp(properties);
            }
            break;

        case ObjectID::Pillar:
            if (propCount == 1) {
                addBackProp(properties);
                addBackProp(properties);
                addBackProp(properties);
            }
            break;

        case ObjectID::TriPlatform:
            if (propCount == 3) {
                addBackProp(properties);
            }
            break;

        default:
            break;
    }
}

void CYWall::verifyPropertyCount()
{
    switch (properties.size()) {
        case 2:
            addBackProp(properties);
            break;
    }
}

ObjectID stringToObjectID(const std::string& objectName)
{
    const static std::unordered_map<std::string, ObjectID> objects{
        {"walls", ObjectID::Wall},          {"floor", ObjectID::Floor},
        {"plat", ObjectID::Platform},       {"triplat", ObjectID::TriPlatform},
        {"diaplat", ObjectID::DiaPlatform}, {"ramp", ObjectID::Ramp},
        {"triwall", ObjectID::TriWall}, {"pillar", ObjectID::Pillar},
        {"door", ObjectID::Door},           {"diamond", ObjectID::Diamond},
        {"monster", ObjectID::Iceman},      {"chaser", ObjectID::Chaser},
        {"hole", ObjectID::Hole},           {"begin", ObjectID::Start},
        {"finish", ObjectID::Finish},       {"jetpack", ObjectID::JetPack},
        {"fuel", ObjectID::Fuel},           {"shield", ObjectID::Shield},
        {"slingshot", ObjectID::Slingshot}, {"crumbs", ObjectID::Crumbs},
        {"teleport", ObjectID::Teleport},   {"key2", ObjectID::Key},
        {"ladder", ObjectID::Ladder},       {"portal", ObjectID::Portal},
        {"board", ObjectID::Message},       {"backmusic", ObjectID::Music},
        {"weather", ObjectID::Weather},     {"theme", ObjectID::Theme},
        {"flight", ObjectID::Unknown},
    };
    return objects.at(objectName);
}

const char* objectIdToString(ObjectID id)
{
    // clang-format off
    const static std::unordered_map<ObjectID, const char*> objects{
        {ObjectID::Wall,        "Wall"},          
        {ObjectID::Floor,       "Level Ground"},
        {ObjectID::Platform,    "Platform"},       
        {ObjectID::TriPlatform, "Platform"},
        {ObjectID::DiaPlatform, "Platform"}, 
        {ObjectID::Ramp,        "Ramp"},
        {ObjectID::TriWall,     "triwall"}, 
        {ObjectID::Pillar,      "Pillar"},
        {ObjectID::Door,        "door"},           
        {ObjectID::Diamond,     "diamond"},
        {ObjectID::Iceman,      "monster"},      
        {ObjectID::Chaser,      "chaser"},
        {ObjectID::Hole,        "Hole"},           
        {ObjectID::Start,       "Spawn Location"},
        {ObjectID::Finish,      "finish"},       
        {ObjectID::JetPack,     "jetpack"},
        {ObjectID::Fuel,        "fuel"},           
        {ObjectID::Shield,      "shield"},
        {ObjectID::Slingshot,   "slingshot"}, 
        {ObjectID::Crumbs,      "crumbs"},
        {ObjectID::Teleport,    "teleport"},   
        {ObjectID::Key,         "key2"},
        {ObjectID::Ladder,      "ladder"},       
        {ObjectID::Portal,      "portal"},
        {ObjectID::Message,     "board"},       
        {ObjectID::Music,       "backmusic"},
        {ObjectID::Weather,     "weather"},     
        {ObjectID::Theme,       "theme"},
        {ObjectID::Unknown,     "flight"},
    };
    // clang-format on
    return objects.at(id);
}

/*
void CYFloor::serialize(BinaryFileBuffer& buffer) const {
    buffer << vertexA << vertexB << vertexC << vertexD << floor
        << (u32)std::stoul(properties[0])
        << (u32)std::stoul(properties[2])
        << (u8)std::stoi(properties[1]);
}

void CYWall::serialize(BinaryFileBuffer& buffer) const {
    buffer << beginPoint << endPoint << floor
        << (u32)std::stoul(properties[0])
        << (u32)std::stoul(properties[1])
        << (u8)std::stoi(properties[2]);
}
*/