#include "UpgradeManager.h"

UpgradeManager& UpgradeManager::Instance()
{
    static auto&& instance = UpgradeManager();
    return instance;
}

int UpgradeManager::getNumShips()
{
    return currentUpgrades[0].currentValue;
}

float UpgradeManager::getShootingRateIncrement()
{
    return currentUpgrades[1].currentValue;
}

int UpgradeManager::getNumBullets()
{
    return currentUpgrades[2].currentValue;
}

float UpgradeManager::getShipsHealthIncrement()
{
    return currentUpgrades[3].currentValue;
}

float UpgradeManager::getDamageIncrement()
{
    return currentUpgrades[4].currentValue;
}

float UpgradeManager::getPlanetHealth()
{
    return currentUpgrades[5].currentValue;
}

float UpgradeManager::getGenericCurrentValue(UpgradeIndex upgradeIndex)
{
    int intUpgradeIndex = static_cast<int>(upgradeIndex);
    return (intUpgradeIndex < 0 || intUpgradeIndex >= currentUpgrades.size()) ? -1 : currentUpgrades[intUpgradeIndex].currentValue;
}

const char * UpgradeManager::getUpgradeName(UpgradeIndex upgradeIndex)
{
    int intUpgradeIndex = static_cast<int>(upgradeIndex);
    return (intUpgradeIndex < 0 || intUpgradeIndex >= currentUpgrades.size()) ? "" : currentUpgrades[intUpgradeIndex].upgradeName;
}

bool UpgradeManager::hasReachedMax(UpgradeIndex upgradeIndex)
{
    int intUpgradeIndex = static_cast<int>(upgradeIndex);
    return (intUpgradeIndex < 0 || intUpgradeIndex >= currentUpgrades.size()) ? true : ((currentUpgrades[intUpgradeIndex].currentValue == currentUpgrades[intUpgradeIndex].maxValue) ? true : false);
}

int UpgradeManager::getUpgradeCost(UpgradeIndex upgradeIndex)
{
    int intUpgradeIndex = static_cast<int>(upgradeIndex);
    return (intUpgradeIndex < 0 || intUpgradeIndex >= currentUpgrades.size()) ? -1 : currentUpgrades[intUpgradeIndex].cost;
}

void UpgradeManager::makeUpgrade(UpgradeIndex upgradeIndex)
{
    if (upgradeIndex < 0 || upgradeIndex >= currentUpgrades.size())
        return;
    if (currentUpgrades[upgradeIndex].currentValue >= currentUpgrades[upgradeIndex].maxValue) return;
    currentUpgrades[upgradeIndex].currentValue += currentUpgrades[upgradeIndex].valueIncrement;
    currentUpgrades[upgradeIndex].cost = currentUpgrades[upgradeIndex].cost * currentUpgrades[upgradeIndex].costIncrementRate;
}

float UpgradeManager::getInitialValue(UpgradeIndex upgradeIndex)
{
    int intUpgradeIndex = static_cast<int>(upgradeIndex);
    return (intUpgradeIndex < 0 || intUpgradeIndex >= currentUpgrades.size()) ? -1 : currentUpgrades[intUpgradeIndex].initialValue;
}
 
void UpgradeManager::reset()
{
	for (auto&& upgrade : currentUpgrades)
	{
		upgrade.currentValue = upgrade.initialValue;
		upgrade.cost = upgrade.initialCost;
	}
}