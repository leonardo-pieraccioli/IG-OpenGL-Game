#pragma once

#include "string"
#include "sstream"
#include "iomanip"
#include "IObserver.h"

class Timer
{
public:
    Timer(std::string name = "", float timeAmount = 5000.0f, bool isTicking = false, bool shouldNotify = false, IObserver* observer = nullptr);
    
    void updateTimer(float deltaTime);

    void setNewTime(float currentTimeAmount, bool isTicking = false);
    float getRemainingTime();
    float getStartingTime();

    void setIsTicking(bool isTicking);
    bool getIsTicking();

    void resetTimer(bool isTicking);
	void resetTimer(float timeAmount, bool isTicking, bool restart);

    std::string getHH_MM_SS_MS();
private:
    float currentTimeAmount;
    float initialTimeAmount;
    bool isTicking;
    bool shouldNotify;
    std::string name;
    IObserver* observer;
};

