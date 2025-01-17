#include "TimerManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

std::map<std::string, Timer*> TimerManager::Timers;


Timer* TimerManager::CreateTimer(const float timeAmount, bool isTicking, std::string name, bool shouldNotify, IObserver* observer)
{
    if (shouldNotify && !observer) {
        std::cerr << "Cannot instantiate a notifying Timer if a valid IObserver isn't passed to this function. Created a not notifying Timer instead\n";
        shouldNotify = false;
    }
    Timers[name] = new Timer(name, timeAmount, isTicking, shouldNotify, observer);
    return Timers[name];
}

Timer* TimerManager::GetTimer(std::string name)
{
    return Timers[name];
}

void TimerManager::updateTimers(float deltaTime)
{
    for (auto& currentTimer : Timers) {
        if (currentTimer.second->getIsTicking())
            currentTimer.second->updateTimer(deltaTime);
    }
}
