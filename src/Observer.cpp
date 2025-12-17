#include "Observer.h"
#include <iostream>

void ConsoleObserver::onKill(const NPC& killer, const NPC& victim) {
    std::cout << "[KILL] " << killer.getTypeName() << " '" << killer.getName() 
              << "' убил " << victim.getTypeName() << " '" << victim.getName() << "'" << std::endl;
}

FileObserver::FileObserver(const std::string& filename) {
    file.open(filename, std::ios::out | std::ios::trunc);
}

FileObserver::~FileObserver() {
    if (file.is_open()) {
        file.close();
    }
}

void FileObserver::onKill(const NPC& killer, const NPC& victim) {
    if (file.is_open()) {
        file << "[KILL] " << killer.getTypeName() << " '" << killer.getName() 
             << "' убил " << victim.getTypeName() << " '" << victim.getName() << "'" << std::endl;
    }
}

void ObserverManager::addObserver(std::shared_ptr<Observer> observer) {
    observers.push_back(observer);
}

void ObserverManager::notifyKill(const NPC& killer, const NPC& victim) {
    for (auto& observer : observers) {
        observer->onKill(killer, victim);
    }
}
