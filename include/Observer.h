#ifndef OBSERVER_H
#define OBSERVER_H

#include "NPC.h"
#include <vector>
#include <fstream>
#include <memory>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onKill(const NPC& killer, const NPC& victim) = 0;
};

class ConsoleObserver : public Observer {
public:
    void onKill(const NPC& killer, const NPC& victim) override;
};

class FileObserver : public Observer {
private:
    std::ofstream file;

public:
    FileObserver(const std::string& filename);
    ~FileObserver();
    void onKill(const NPC& killer, const NPC& victim) override;
};

class ObserverManager {
private:
    std::vector<std::shared_ptr<Observer>> observers;

public:
    void addObserver(std::shared_ptr<Observer> observer);
    void notifyKill(const NPC& killer, const NPC& victim);
};

#endif