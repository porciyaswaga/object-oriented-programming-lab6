#ifndef NPC_H
#define NPC_H

#include <string>
#include <memory>

enum class NPCType {
    Knight,
    Druid,
    Elf
};

class Visitor;

class NPC {
protected:
    NPCType type;
    std::string name;
    int x, y;
    bool alive;

public:
    NPC(NPCType t, const std::string& n, int x, int y);
    virtual ~NPC() = default;

    NPCType getType() const;
    std::string getName() const;
    int getX() const;
    int getY() const;
    bool isAlive() const;
    void kill();

    virtual void accept(Visitor& visitor, NPC& other) = 0;
    virtual std::string getTypeName() const = 0;

    double distanceTo(const NPC& other) const;

    virtual std::string serialize() const;
};

class Knight : public NPC {
public:
    Knight(const std::string& name, int x, int y);
    void accept(Visitor& visitor, NPC& other) override;
    std::string getTypeName() const override;
};

class Druid : public NPC {
public:
    Druid(const std::string& name, int x, int y);
    void accept(Visitor& visitor, NPC& other) override;
    std::string getTypeName() const override;
};

class Elf : public NPC {
public:
    Elf(const std::string& name, int x, int y);
    void accept(Visitor& visitor, NPC& other) override;
    std::string getTypeName() const override;
};

#endif