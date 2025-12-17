#include "NPC.h"
#include "NPCFactory.h"
#include "Observer.h"
#include "Visitor.h"
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

void printNPCs(const std::vector<std::shared_ptr<NPC>>& npcs) {
    std::cout << "\n=== Список NPC ===" << std::endl;
    int count = 0;
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            std::cout << ++count << ". " << npc->getTypeName() 
                      << " '" << npc->getName() << "' "
                      << "(" << npc->getX() << ", " << npc->getY() << ")" << std::endl;
        }
    }
    std::cout << "Всего живых: " << count << std::endl;
    std::cout << "==================\n" << std::endl;
}

void runBattle(std::vector<std::shared_ptr<NPC>>& npcs, double range, ObserverManager& manager) {
    std::cout << "\n=== Боевой режим (дальность: " << range << ") ===" << std::endl;
    
    BattleVisitor visitor(range, manager);
    
    for (auto& attacker : npcs) {
        for (auto& defender : npcs) {
            if (attacker != defender && attacker->isAlive() && defender->isAlive()) {
                attacker->accept(visitor, *defender);
            }
        }
    }
    
    visitor.applyKills();
    
    npcs.erase(
        std::remove_if(npcs.begin(), npcs.end(), 
            [](const std::shared_ptr<NPC>& npc) { return !npc->isAlive(); }),
        npcs.end()
    );
    
    std::cout << "=== Бой завершен ===\n" << std::endl;
}

int main() {
    std::cout << "=== Редактор подземелья Balagur Fate 3 ===" << std::endl;
    std::cout << "Вариант 11: Рыцарь, Друид, Эльф" << std::endl;
    std::cout << "Правила:" << std::endl;
    std::cout << "  - Рыцарь убивает эльфа" << std::endl;
    std::cout << "  - Эльф убивает друида и рыцаря" << std::endl;
    std::cout << "  - Друид убивает друидов" << std::endl;

    // Создаем наблюдателей
    ObserverManager observerManager;
    observerManager.addObserver(std::make_shared<ConsoleObserver>());
    observerManager.addObserver(std::make_shared<FileObserver>("log.txt"));

    // Создаем 20 NPC
    std::vector<std::shared_ptr<NPC>> npcs;
    
    // 7 рыцарей
    npcs.push_back(NPCFactory::createNPC(NPCType::Knight, "Arthur", 50, 50));
    npcs.push_back(NPCFactory::createNPC(NPCType::Knight, "Lancelot", 100, 100));
    npcs.push_back(NPCFactory::createNPC(NPCType::Knight, "Galahad", 150, 80));
    npcs.push_back(NPCFactory::createNPC(NPCType::Knight, "Percival", 200, 200));
    npcs.push_back(NPCFactory::createNPC(NPCType::Knight, "Gawain", 300, 150));
    npcs.push_back(NPCFactory::createNPC(NPCType::Knight, "Tristan", 400, 400));
    npcs.push_back(NPCFactory::createNPC(NPCType::Knight, "Bors", 450, 100));

    // 7 эльфов
    npcs.push_back(NPCFactory::createNPC(NPCType::Elf, "Legolas", 60, 60));
    npcs.push_back(NPCFactory::createNPC(NPCType::Elf, "Thranduil", 120, 90));
    npcs.push_back(NPCFactory::createNPC(NPCType::Elf, "Elrond", 180, 180));
    npcs.push_back(NPCFactory::createNPC(NPCType::Elf, "Galadriel", 250, 250));
    npcs.push_back(NPCFactory::createNPC(NPCType::Elf, "Arwen", 320, 140));
    npcs.push_back(NPCFactory::createNPC(NPCType::Elf, "Celeborn", 380, 380));
    npcs.push_back(NPCFactory::createNPC(NPCType::Elf, "Glorfindel", 480, 120));

    // 6 друидов (некоторые рядом друг с другом)
    npcs.push_back(NPCFactory::createNPC(NPCType::Druid, "Merlin", 70, 70));
    npcs.push_back(NPCFactory::createNPC(NPCType::Druid, "Getafix", 90, 85));      // рядом с Merlin
    npcs.push_back(NPCFactory::createNPC(NPCType::Druid, "Radagast", 210, 190));
    npcs.push_back(NPCFactory::createNPC(NPCType::Druid, "Taliesin", 230, 200));   // рядом с Radagast
    npcs.push_back(NPCFactory::createNPC(NPCType::Druid, "Cathbad", 350, 160));
    npcs.push_back(NPCFactory::createNPC(NPCType::Druid, "Amergin", 365, 175));

    // Показываем всех NPC до боя
    std::cout << "\n--- ДО БОЯ ---";
    printNPCs(npcs);

    // Сохраняем в файл
    NPCFactory::saveToFile(npcs, "npcs.txt");
    std::cout << "NPC сохранены в файл npcs.txt" << std::endl;

    // Запускаем бой с дальностью 50 метров
    runBattle(npcs, 50.0, observerManager);

    // Показываем выживших
    std::cout << "--- ПОСЛЕ БОЯ ---";
    printNPCs(npcs);

    // Сохраняем выживших
    NPCFactory::saveToFile(npcs, "survivors.txt");
    std::cout << "Выжившие сохранены в файл survivors.txt" << std::endl;

    return 0;
}
