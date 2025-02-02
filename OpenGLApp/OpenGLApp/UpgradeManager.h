#pragma once
#include "array"

class UpgradeManager
{
public:
	typedef struct {
		float currentValue;
		float maxValue;
		int cost;
		float costIncrementRate;
	}upgradeType;
private:
	UpgradeManager() {}

	// 0: Increase number of ships
	// 1: Increase shootingRate
	// 2: Increase number of bullets shot
	// 3: Increase max ships health
	// 4: Increase damage
	std::array<upgradeType, 5> currentUpgrades = {{
		{1.0f, 4.0f, 500, 150.f / 100.f},
		{1.0f, 5.0f, 500, 150.f / 100.f},
		{1.0f, 5.0f, 600, 200.f / 100.f},
		{100.f, 1000.f, 100, 130.f / 100.f},
		{25.f, 250.f, 250, 150.f / 100.f}
	}};

public:
	static UpgradeManager& Instance();

	int getNumShips();
	int getShootingRateIncrement();
	int getNumBullets();
	int getShipsHealthIncrement();
	int getDamageIncrement();
	int getGenericCurrentValue(int upgradeIndex);
	bool hasReachedMax(int upgradeIndex);
	int getUpgradeCost(int upgradeIndex);
	void makeUpgrade(int upgradeIndex);
};

