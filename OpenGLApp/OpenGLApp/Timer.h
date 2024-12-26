#pragma once

#include "string"
#include "sstream"
#include "iomanip"

class Timer
{
public:
    Timer(float timeAmount = 5000.0f, bool isTicking = false);
    
    void updateTimer(float deltaTime);

    void setNewTime(float currentTimeAmount, bool isTicking = false);
    float getRemainingTime();

    void setIsTicking(bool isTicking);
    bool getIsTicking();

    void resetTimer(bool isTicking);

    std::string getHH_MM_SS_MS();
private:
    float currentTimeAmount;
    float initialTimeAmount;
    bool isTicking;
};

