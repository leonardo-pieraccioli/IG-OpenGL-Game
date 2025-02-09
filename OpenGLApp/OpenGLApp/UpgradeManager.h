#pragma once
#include "array"

enum class UpgradeIndex { 
	ShipsNumber, 
	ShootingRate, 
	BulletsNumber, 
	MaxShipsHealth, 
	Damage,
	PlanetHealth
};

class UpgradeManager
{
public:
	typedef struct {
		float initialValue;
		float currentValue;
		float valueIncrement;
		float maxValue;
		int cost;
		float costIncrementRate;
	}upgradeType;
private:
	UpgradeManager() {}

	// init_val		curr_val	val_incr	max_val		cost	cost_increment_rate
	std::array<upgradeType, 6> currentUpgrades = {{
		{1.0f,		1.0f,		1.f,		4.0f,		500,	1.5f},				// 0: Increase number of ships
		{1.0f,		1.0f,		0.5f,		5.0f,		500,	150.f / 100.f},		// 1: Increase shootingRate
		{1.0f,		1.0f,		2.f,		5.0f,		600,	200.f / 100.f},		// 2: Increase number of bullets shot
		{100.f,		100.f,		50.f,		1000.f,		100,	130.f / 100.f},		// 3: Increase max ships health
		{25.f,		25.f,		1.f,		250.f,		250,	150.f / 100.f},		// 4: Increase damage
		{100.f,		100.f,		20.f,		200.f,		300,	1.7f},				// 5: Increase maximum planet health
	}};

public:
	static UpgradeManager& Instance();

	int getNumShips();
	int getShootingRateIncrement();
	int getNumBullets();
	int getShipsHealthIncrement();
	int getDamageIncrement();
	int getGenericCurrentValue(UpgradeIndex upgradeIndex);
	bool hasReachedMax(UpgradeIndex upgradeIndex);
	int getUpgradeCost(UpgradeIndex upgradeIndex);
	void makeUpgrade(UpgradeIndex upgradeIndex);
};

