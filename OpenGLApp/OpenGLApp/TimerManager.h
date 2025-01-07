#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "Timer.h"

class TimerManager
{
public:
    // Timer storage
    static std::map<std::string, Timer> Timers;

    // creates a new timer
    static Timer CreateTimer(const float timeAmount, bool isTicking, std::string name, bool shouldNotify, IObserver* observer);

    // retrieves a stored timer
    static Timer GetTimer(std::string name);

    static void updateTimers(float deltaTime);
private:
    TimerManager() {}
};

