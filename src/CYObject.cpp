#include "CYObject.h"

#include <unordered_map>
#include <iostream>

namespace {
    void addBackProp(std::vector<std::string>& propList) {
        propList.push_back("1");
    }

    void addFrontProp(std::vector<std::string>& propList) {
        propList.insert(propList.begin(), "1");
    }
}

void CYObject::verifyPropertyCount(ObjectID id) {
    int propCount = properties.size();

    switch(id) {
        case ObjectID::Platform:
            switch(propCount) {
                case 1:
                    addFrontProp(properties);
                case 2: //Fallthrough is intentional
                    addBackProp(properties);
            }
            break;

        case ObjectID::Finish:
            switch (propCount) {
                case 2:
                    properties.erase(properties.begin());
                    break;
            }
            break;
        case ObjectID::Diamond:
        case ObjectID::Iceman:
        case ObjectID::Ramp:
            switch (propCount) {
                case 1:
                    addBackProp(properties);
                    break;
            }
            break;

        case ObjectID::Message:
        case ObjectID::DiaPlatform:
        case ObjectID::Door:
            switch (propCount) {
                case 2:
                    addBackProp(properties);
                    break;
            }
            break;

        case ObjectID::Pillar:
            switch (propCount) {
                case 1:
                    addBackProp(properties);
                    addBackProp(properties);
                    addBackProp(properties);
                    break;
            }
            break;

        case ObjectID::TriPlatform:
            switch(propCount) {
                case 3:
                    addBackProp(properties);
                    break;
            }
            break;

        default:
            break;
    }
}

void CYWall::verifyPropertyCount() {
    switch (properties.size()) {
        case 2:
            addBackProp(properties);
            break;
    }
}

ObjectID stringToObjectID(const std::string& objectName) {
    const static std::unordered_map<std::string, ObjectID> objects {
        {"walls",       ObjectID::Wall          },
        {"floor",       ObjectID::Floor         },
        {"plat",        ObjectID::Platform      },
        {"triplat",     ObjectID::TriPlatform   },
        {"diaplat",     ObjectID::DiaPlatform   },
        {"ramp",        ObjectID::Ramp          },
        {"triwall",     ObjectID::TriPlatform   },
        {"pillar",      ObjectID::Pillar        },
        {"door",        ObjectID::Door          },
        {"diamond",     ObjectID::Diamond       },
        {"monster",     ObjectID::Iceman        },
        {"chaser",      ObjectID::Chaser        },
        {"hole",        ObjectID::Hole          },
        {"begin",       ObjectID::Start         },
        {"finish",      ObjectID::Finish        },
        {"jetpack",     ObjectID::JetPack       },
        {"fuel",        ObjectID::Fuel          },
        {"shield",      ObjectID::Shield        },
        {"slingshot",   ObjectID::Slingshot     },
        {"crumbs",      ObjectID::Crumbs        },
        {"teleport",    ObjectID::Teleport      },
        {"key2",        ObjectID::Key           },
        {"ladder",      ObjectID::Ladder        },
        {"portal",      ObjectID::Portal        },
        {"board",       ObjectID::Message       },

        {"backmusic",   ObjectID::Music         },
        {"weather",     ObjectID::Weather       },
        {"theme",       ObjectID::Theme         },
        {"flight",      ObjectID::Unknown       },
    };
    return objects.at(objectName);
}

void CYFloor::serialize(BinaryFileBuffer& buffer) const {
    buffer  << vertexA << vertexB << vertexC << vertexD << floor 
            << (u32)std::stoul(properties[0]) 
            << (u32)std::stoul(properties[2]) 
            << (u8 )std::stoi(properties[1]);
}

void CYWall::serialize(BinaryFileBuffer& buffer) const {
    buffer  << beginPoint << endPoint << floor
            << (u32)std::stoul(properties[0]) 
            << (u32)std::stoul(properties[1]) 
            << (u8 )std::stoi(properties[2]);
}