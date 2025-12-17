#ifndef VISITOR_H
#define VISITOR_H

#include "NPC.h"
#include "Observer.h"
#include <vector>
#include <memory>

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(Knight& knight, NPC& other) = 0;
    virtual void visit(Druid& druid, NPC& other) = 0;
    virtual void visit(Elf& elf, NPC& other) = 0;
};

class BattleVisitor : public Visitor {
private:
    double battleRange;
    ObserverManager& observerManager;
    std::vector<std::pair<NPC*, NPC*>> kills;

public:
    BattleVisitor(double range, ObserverManager& manager);
    
    void visit(Knight& knight, NPC& other) override;
    void visit(Druid& druid, NPC& other) override;
    void visit(Elf& elf, NPC& other) override;
    void applyKills();
    void clearKills();
};

#endif
