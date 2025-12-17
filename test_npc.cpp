#include <gtest/gtest.h>
#include "NPC.h"
#include "NPCFactory.h"
#include "Observer.h"
#include "Visitor.h"
#include <memory>
#include <fstream>
#include <cstdio>

TEST(NPCFactoryTest, CreateKnight) {
    auto knight = NPCFactory::createNPC(NPCType::Knight, "Arthur", 100, 200);
    EXPECT_EQ(knight->getType(), NPCType::Knight);
    EXPECT_EQ(knight->getName(), "Arthur");
    EXPECT_EQ(knight->getX(), 100);
    EXPECT_EQ(knight->getY(), 200);
    EXPECT_TRUE(knight->isAlive());
}

TEST(NPCFactoryTest, CreateDruid) {
    auto druid = NPCFactory::createNPC(NPCType::Druid, "Merlin", 50, 50);
    EXPECT_EQ(druid->getType(), NPCType::Druid);
    EXPECT_EQ(druid->getName(), "Merlin");
    EXPECT_EQ(druid->getTypeName(), "Druid");
}

TEST(NPCFactoryTest, CreateElf) {
    auto elf = NPCFactory::createNPC(NPCType::Elf, "Legolas", 300, 400);
    EXPECT_EQ(elf->getType(), NPCType::Elf);
    EXPECT_EQ(elf->getName(), "Legolas");
    EXPECT_EQ(elf->getTypeName(), "Elf");
}

TEST(NPCFactoryTest, CreateByString) {
    auto knight = NPCFactory::createNPC("Knight", "Test", 0, 0);
    EXPECT_EQ(knight->getType(), NPCType::Knight);
    
    auto druid = NPCFactory::createNPC("Druid", "Test2", 100, 100);
    EXPECT_EQ(druid->getType(), NPCType::Druid);
    
    auto elf = NPCFactory::createNPC("Elf", "Test3", 200, 200);
    EXPECT_EQ(elf->getType(), NPCType::Elf);
}

TEST(NPCFactoryTest, InvalidCoordinates) {
    EXPECT_THROW(NPCFactory::createNPC(NPCType::Knight, "Test", -1, 0), std::invalid_argument);
    EXPECT_THROW(NPCFactory::createNPC(NPCType::Knight, "Test", 0, 501), std::invalid_argument);
    EXPECT_THROW(NPCFactory::createNPC(NPCType::Knight, "Test", 600, 0), std::invalid_argument);
}

TEST(NPCFactoryTest, InvalidType) {
    EXPECT_THROW(NPCFactory::createNPC("InvalidType", "Test", 0, 0), std::invalid_argument);
}

TEST(NPCTest, Distance) {
    auto npc1 = NPCFactory::createNPC(NPCType::Knight, "A", 0, 0);
    auto npc2 = NPCFactory::createNPC(NPCType::Elf, "B", 3, 4);
    
    EXPECT_DOUBLE_EQ(npc1->distanceTo(*npc2), 5.0);
}

TEST(NPCTest, DistanceZero) {
    auto npc1 = NPCFactory::createNPC(NPCType::Knight, "A", 100, 100);
    auto npc2 = NPCFactory::createNPC(NPCType::Elf, "B", 100, 100);
    
    EXPECT_DOUBLE_EQ(npc1->distanceTo(*npc2), 0.0);
}

TEST(NPCTest, Serialize) {
    auto knight = NPCFactory::createNPC(NPCType::Knight, "Arthur", 150, 250);
    EXPECT_EQ(knight->serialize(), "Knight Arthur 150 250");
}

TEST(NPCFactoryTest, SaveAndLoad) {
    std::vector<std::shared_ptr<NPC>> npcs;
    npcs.push_back(NPCFactory::createNPC(NPCType::Knight, "Knight1", 100, 100));
    npcs.push_back(NPCFactory::createNPC(NPCType::Druid, "Druid1", 200, 200));
    npcs.push_back(NPCFactory::createNPC(NPCType::Elf, "Elf1", 300, 300));
    
    const std::string filename = "test_save.txt";
    NPCFactory::saveToFile(npcs, filename);
    
    auto loaded = NPCFactory::loadFromFile(filename);
    EXPECT_EQ(loaded.size(), 3);
    EXPECT_EQ(loaded[0]->getType(), NPCType::Knight);
    EXPECT_EQ(loaded[1]->getType(), NPCType::Druid);
    EXPECT_EQ(loaded[2]->getType(), NPCType::Elf);
    
    std::remove(filename.c_str());
}

TEST(BattleTest, KnightKillsElf) {
    ObserverManager manager;
    BattleVisitor visitor(100.0, manager);
    
    auto knight = std::make_shared<Knight>("Knight1", 0, 0);
    auto elf = std::make_shared<Elf>("Elf1", 10, 10);
    
    knight->accept(visitor, *elf);
    visitor.applyKills();
    
    EXPECT_TRUE(knight->isAlive());
    EXPECT_FALSE(elf->isAlive());
}

TEST(BattleTest, ElfKillsDruid) {
    ObserverManager manager;
    BattleVisitor visitor(100.0, manager);
    
    auto elf = std::make_shared<Elf>("Elf1", 0, 0);
    auto druid = std::make_shared<Druid>("Druid1", 10, 10);
    
    elf->accept(visitor, *druid);
    visitor.applyKills();
    
    EXPECT_TRUE(elf->isAlive());
    EXPECT_FALSE(druid->isAlive());
}

TEST(BattleTest, ElfKillsKnight) {
    ObserverManager manager;
    BattleVisitor visitor(100.0, manager);
    
    auto elf = std::make_shared<Elf>("Elf1", 0, 0);
    auto knight = std::make_shared<Knight>("Knight1", 10, 10);
    
    elf->accept(visitor, *knight);
    visitor.applyKills();
    
    EXPECT_TRUE(elf->isAlive());
    EXPECT_FALSE(knight->isAlive());
}

TEST(BattleTest, DruidKillsDruid) {
    ObserverManager manager;
    BattleVisitor visitor(100.0, manager);
    
    auto druid1 = std::make_shared<Druid>("Druid1", 0, 0);
    auto druid2 = std::make_shared<Druid>("Druid2", 10, 10);
    
    druid1->accept(visitor, *druid2);
    visitor.applyKills();
    
    EXPECT_TRUE(druid1->isAlive());
    EXPECT_FALSE(druid2->isAlive());
}

TEST(BattleTest, KnightDoesNotKillDruid) {
    ObserverManager manager;
    BattleVisitor visitor(100.0, manager);
    
    auto knight = std::make_shared<Knight>("Knight1", 0, 0);
    auto druid = std::make_shared<Druid>("Druid1", 10, 10);
    
    knight->accept(visitor, *druid);
    visitor.applyKills();
    
    EXPECT_TRUE(knight->isAlive());
    EXPECT_TRUE(druid->isAlive());
}

TEST(BattleTest, ElfDoesNotKillElf) {
    ObserverManager manager;
    BattleVisitor visitor(100.0, manager);
    
    auto elf1 = std::make_shared<Elf>("Elf1", 0, 0);
    auto elf2 = std::make_shared<Elf>("Elf2", 10, 10);
    
    elf1->accept(visitor, *elf2);
    visitor.applyKills();
    
    EXPECT_TRUE(elf1->isAlive());
    EXPECT_TRUE(elf2->isAlive());
}

TEST(BattleTest, OutOfRange) {
    ObserverManager manager;
    BattleVisitor visitor(10.0, manager);
    
    auto knight = std::make_shared<Knight>("Knight1", 0, 0);
    auto elf = std::make_shared<Elf>("Elf1", 100, 100);
    
    knight->accept(visitor, *elf);
    visitor.applyKills();
    
    EXPECT_TRUE(knight->isAlive());
    EXPECT_TRUE(elf->isAlive());
}

TEST(NPCTest, KillMethod) {
    auto npc = NPCFactory::createNPC(NPCType::Knight, "Test", 0, 0);
    EXPECT_TRUE(npc->isAlive());
    npc->kill();
    EXPECT_FALSE(npc->isAlive());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
