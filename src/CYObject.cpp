#include "CYObject.h"

namespace {
    void addBackProp(std::vector<std::string>& propList) {
        propList.push_back("1");
    }

    void addFrontProp(std::vector<std::string>& propList) {
        propList.insert(propList.begin(), "1");
    }
}

void CYObject::verifyPropertyCount(const std::string& name) {
    int propCount = properties.size();
    if (name == "plat") {
        switch (propCount) {
            case 1:
                addFrontProp(properties);
            case 2:
                addBackProp(properties);
                break;
        }
    }
    else if (name == "diamond" || name == "finish" || name == "monster" || name == "ramp") {
        switch (propCount) {
            case 1:
               addBackProp(properties);
               break;
        }
    }
    else if (name == "board" || name == "diaplat" || name == "door") {
        switch (propCount) {
            case 2:
                addBackProp(properties);
        }
    }
    else if (name == "pillar") {
        switch (propCount) {
            case 1:
                addFrontProp(properties);
                addBackProp(properties);
                addBackProp(properties);
                break;
        }
    }
    else if (name == "triplat") {
        switch(propCount) {
            case 3:
                addBackProp(properties);
        }
    }
}