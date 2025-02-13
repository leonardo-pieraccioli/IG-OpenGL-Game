#pragma once
#include "array"
#define TOT_UPGRADES 6
enum UpgradeIndex: int{
	ShipsNumber		= 0,	// Player
	ShootingRate	= 1,	// Player
	BulletsNumber	= 2,	// Ship
	MaxShipsHealth	= 3,	// Ship
	Damage			= 4,	// Ship
	PlanetHealth	= 5		// Planet
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
		int cost;
		float costIncrementRate;
	}upgradeType;
private:
	UpgradeManager() {}

	//	upgrade_name			init_val	curr_val	val_incr	max_val		cost	cost_increment_rate
	std::array<upgradeType,	TOT_UPGRADES> currentUpgrades = {{
		{"Number of ships",		1.0f,		1.0f,		1.f,		4.0f,		500,	1.5f},
		{"Shooting rate",		1.0f,		1.0f,		0.5f,		5.0f,		500,	1.5f},
		{"Bullets number",		1.0f,		1.0f,		2.f,		5.0f,		600,	2.f},
		{"Ship max health",		100.f,		100.f,		50.f,		1000.f,		100,	1.5f},
		{"Laser damage",		10.f,		10.f,		5.f,		50.f,		250,	1.2f},
		{"Planet max health",	100.f,		100.f,		20.f,		200.f,		300,	1.7f},
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
};

