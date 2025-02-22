#pragma once
#include "array"
#define TOT_UPGRADES 7

enum UpgradeIndex: int{
	ShipsNumber		= 0,	// Player
	ShootingRate	= 1,	// Player
	BulletsNumber	= 2,	// Ship
	MaxShipsHealth	= 3,	// Ship
	Damage			= 4,	// Ship
	PlanetHealth	= 5,	// Planet
	ShipsSpeed		= 6		// Ship
};

class UpgradeManager
{
public:
	typedef struct {
		const char* upgradeName;
		float initialValue;
		float currentValue;
		float valueIncrement;
		float maxValue;
		int initialCost;
		int cost;
		float costIncrementRate;
	}upgradeType;
private:
	UpgradeManager() {}

	//	upgrade_name			init_val	curr_val	val_incr	max_val		init_cost	cost	cost_increment_rate
	std::array<upgradeType,	TOT_UPGRADES> currentUpgrades = {{
		{"Number of ships",		1.0f,		1.0f,		1.f,		4.0f,		1200,		1200,	2.0f},
		{"Shooting rate",		1.0f,		1.0f,		0.50f,		7.0f,		400,		400,	1.5f},
		{"Bullets number",		1.0f,		1.0f,		2.f,		5.0f,		1000,		1000,	2.5f},
		{"Ship max health",		80.f,		80.f,		20.f,		200.f,		150,		150,	1.5f},
		{"Laser damage",		10.f,		10.f,		7.5f,		85.f,		200,		200,	1.3f},
		{"Planet max health",	100.f,		100.f,		50.f,		400.f,		400,		400,	1.8f},
		{"Ship rotation speed",	95.f,		95.f,		5.f,		130.f,		220,		220,	1.5f}
	}};

public:
	static UpgradeManager& Instance();

	int getNumShips();
	float getShootingRateIncrement();
	int getNumBullets();
	float getShipsHealthIncrement();
	float getDamageIncrement();
	float getPlanetHealth();
	float getGenericCurrentValue(UpgradeIndex upgradeIndex);
	const char * getUpgradeName(UpgradeIndex upgradeIndex);
	bool hasReachedMax(UpgradeIndex upgradeIndex);
	int getUpgradeCost(UpgradeIndex upgradeIndex);
	void makeUpgrade(UpgradeIndex upgradeIndex);
	float getInitialValue(UpgradeIndex upgradeIndex);
	void reset();
};

