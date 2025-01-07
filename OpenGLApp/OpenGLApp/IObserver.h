#pragma once
#include <string>

class IObserver {
public:
	virtual void getNotified(std::string timerName, bool isCallbackEnabled) = 0;
};