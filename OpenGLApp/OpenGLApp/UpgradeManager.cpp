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

int UpgradeManager::getShootingRateIncrement()
{
    return currentUpgrades[1].currentValue;
}

int UpgradeManager::getNumBullets()
{
    return currentUpgrades[2].currentValue;
}

int UpgradeManager::getShipsHealthIncrement()
{
    return currentUpgrades[3].currentValue;
}

int UpgradeManager::getDamageIncrement()
{
    return currentUpgrades[4].currentValue;
}

int UpgradeManager::getGenericCurrentValue(int upgradeIndex)
{
    return (upgradeIndex < 0 || upgradeIndex >= currentUpgrades.size()) ? -1 : currentUpgrades[upgradeIndex].currentValue;
}

bool UpgradeManager::hasReachedMax(int upgradeIndex)
{
    return (upgradeIndex < 0 || upgradeIndex >= currentUpgrades.size()) ? true : ((currentUpgrades[upgradeIndex].currentValue == currentUpgrades[upgradeIndex].maxValue) ? true : false);
}

int UpgradeManager::getUpgradeCost(int upgradeIndex)
{
    return (upgradeIndex < 0 || upgradeIndex >= currentUpgrades.size()) ? -1 : currentUpgrades[upgradeIndex].cost;
}

void UpgradeManager::makeUpgrade(int upgradeIndex)
{
    if (upgradeIndex < 0 || upgradeIndex >= currentUpgrades.size())
        return;
    currentUpgrades[upgradeIndex].cost = currentUpgrades[upgradeIndex].cost * currentUpgrades[upgradeIndex].costIncrementRate;
}
