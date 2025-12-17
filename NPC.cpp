#include "NPC.h"
#include "Visitor.h"
#include <cmath>
#include <sstream>

// Базовый класс NPC
NPC::NPC(NPCType t, const std::string& n, int x, int y)
    : type(t), name(n), x(x), y(y), alive(true) {}

NPCType NPC::getType() const { return type; }
std::string NPC::getName() const { return name; }
int NPC::getX() const { return x; }
int NPC::getY() const { return y; }
bool NPC::isAlive() const { return alive; }
void NPC::kill() { alive = false; }

double NPC::distanceTo(const NPC& other) const {
    double dx = x - other.x;
    double dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
}

std::string NPC::serialize() const {
    std::stringstream ss;
    ss << getTypeName() << " " << name << " " << x << " " << y;
    return ss.str();
}

// Странствующий рыцарь
Knight::Knight(const std::string& name, int x, int y)
    : NPC(NPCType::Knight, name, x, y) {}

void Knight::accept(Visitor& visitor, NPC& other) {
    visitor.visit(*this, other);
}

std::string Knight::getTypeName() const { return "Knight"; }

// Друид
Druid::Druid(const std::string& name, int x, int y)
    : NPC(NPCType::Druid, name, x, y) {}

void Druid::accept(Visitor& visitor, NPC& other) {
    visitor.visit(*this, other);
}

std::string Druid::getTypeName() const { return "Druid"; }

// Эльф
Elf::Elf(const std::string& name, int x, int y)
    : NPC(NPCType::Elf, name, x, y) {}

void Elf::accept(Visitor& visitor, NPC& other) {
    visitor.visit(*this, other);
}

std::string Elf::getTypeName() const { return "Elf"; }
