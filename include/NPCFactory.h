#ifndef NPC_FACTORY_H
#define NPC_FACTORY_H

#include "NPC.h"
#include <memory>
#include <vector>
#include <string>

class NPCFactory {
public:
    static std::shared_ptr<NPC> createNPC(NPCType type, const std::string& name, int x, int y);
    
    static std::shared_ptr<NPC> createNPC(const std::string& type, const std::string& name, int x, int y);
    
    static std::vector<std::shared_ptr<NPC>> loadFromFile(const std::string& filename);
    
    static void saveToFile(const std::vector<std::shared_ptr<NPC>>& npcs, const std::string& filename);
};

#endif