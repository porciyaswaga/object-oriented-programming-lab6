#include "NPCFactory.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::shared_ptr<NPC> NPCFactory::createNPC(NPCType type, const std::string& name, int x, int y) {
    if (x < 0 || x > 500 || y < 0 || y > 500) {
        throw std::invalid_argument("Coordinates must be in range [0, 500]");
    }
    
    switch (type) {
        case NPCType::Knight:
            return std::make_shared<Knight>(name, x, y);
        case NPCType::Druid:
            return std::make_shared<Druid>(name, x, y);
        case NPCType::Elf:
            return std::make_shared<Elf>(name, x, y);
        default:
            throw std::invalid_argument("Unknown NPC type");
    }
}

std::shared_ptr<NPC> NPCFactory::createNPC(const std::string& type, const std::string& name, int x, int y) {
    if (type == "Knight") {
        return createNPC(NPCType::Knight, name, x, y);
    } else if (type == "Druid") {
        return createNPC(NPCType::Druid, name, x, y);
    } else if (type == "Elf") {
        return createNPC(NPCType::Elf, name, x, y);
    } else {
        throw std::invalid_argument("Unknown NPC type: " + type);
    }
}

std::vector<std::shared_ptr<NPC>> NPCFactory::loadFromFile(const std::string& filename) {
    std::vector<std::shared_ptr<NPC>> npcs;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        std::string type, name;
        int x, y;
        
        if (iss >> type >> name >> x >> y) {
            try {
                npcs.push_back(createNPC(type, name, x, y));
            } catch (const std::exception& e) {
            }
        }
    }
    
    return npcs;
}

void NPCFactory::saveToFile(const std::vector<std::shared_ptr<NPC>>& npcs, const std::string& filename) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }
    
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            file << npc->serialize() << "\n";
        }
    }
}
