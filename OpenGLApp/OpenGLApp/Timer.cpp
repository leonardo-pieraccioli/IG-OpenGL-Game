#include "Timer.h"

Timer::Timer(float timeAmount, bool isTicking)
{
	currentTimeAmount = timeAmount;
	initialTimeAmount = timeAmount;
	this->isTicking = isTicking;
}

void Timer::updateTimer(float deltaTime)
{
	currentTimeAmount = currentTimeAmount - deltaTime <= 0.f ? 0.0f : currentTimeAmount - deltaTime;
	if (currentTimeAmount <= 0.f)
		isTicking = false;
}

void Timer::setNewTime(float currentTimeAmount, bool isTicking)
{
	this->currentTimeAmount = currentTimeAmount;
	this->isTicking = isTicking;
}

float Timer::getRemainingTime()
{
	return currentTimeAmount;
}

void Timer::setIsTicking(bool isTicking)
{
	this->isTicking = isTicking;
}

bool Timer::getIsTicking()
{
	return isTicking;
}

void Timer::resetTimer(bool isTicking)
{
	currentTimeAmount = initialTimeAmount;
	this->isTicking = isTicking;
}

std::string Timer::getHH_MM_SS_MS()
{
	int hours, minutes, seconds, milliseconds;

	int currentSeconds = floor(currentTimeAmount);

	hours = currentSeconds / 3600;
	minutes = (currentSeconds % 3600) / 60;
	seconds = (currentSeconds % 3600) % 60;
	milliseconds = (currentTimeAmount - floor(currentTimeAmount))*1000;

	std::stringstream returnStringStream;
	returnStringStream << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds << "." << std::setw(3) << std::setfill('0') << milliseconds;
	std::string returnString = returnStringStream.str();

	return returnString;
}
