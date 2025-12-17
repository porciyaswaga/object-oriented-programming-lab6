#include "Visitor.h"
#include <iostream>

BattleVisitor::BattleVisitor(double range, ObserverManager& manager)
    : battleRange(range), observerManager(manager) {}

void BattleVisitor::visit(Knight& knight, NPC& other) {
    if (!knight.isAlive() || !other.isAlive()) return;
    if (knight.distanceTo(other) > battleRange) return;
    
    if (other.getType() == NPCType::Elf) {
        kills.push_back({&knight, &other});
    }
}

void BattleVisitor::visit(Druid& druid, NPC& other) {
    if (!druid.isAlive() || !other.isAlive()) return;
    if (druid.distanceTo(other) > battleRange) return;
    
    if (other.getType() == NPCType::Druid && &druid != &other) {
        kills.push_back({&druid, &other});
    }
}

void BattleVisitor::visit(Elf& elf, NPC& other) {
    if (!elf.isAlive() || !other.isAlive()) return;
    if (elf.distanceTo(other) > battleRange) return;
    
    if (other.getType() == NPCType::Druid || other.getType() == NPCType::Knight) {
        kills.push_back({&elf, &other});
    }
}

void BattleVisitor::applyKills() {
    for (auto& pair : kills) {
        if (pair.first->isAlive() && pair.second->isAlive()) {
            observerManager.notifyKill(*pair.first, *pair.second);
            pair.second->kill();
        }
    }
}

void BattleVisitor::clearKills() {
    kills.clear();
}
