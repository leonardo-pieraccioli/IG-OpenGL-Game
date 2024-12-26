#include "TimerManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

std::map<std::string, Timer> TimerManager::Timers;


Timer TimerManager::CreateTimer(const float timeAmount, bool isTicking, std::string name)
{
    Timers[name] = Timer(timeAmount, isTicking);
    return Timers[name];
}

Timer TimerManager::GetTimer(std::string name)
{
    return Timers[name];
}

void TimerManager::updateTimers(float deltaTime)
{
    for (auto& currentTimer : Timers) {
        if (currentTimer.second.getIsTicking())
            currentTimer.second.updateTimer(deltaTime);
    }
}
