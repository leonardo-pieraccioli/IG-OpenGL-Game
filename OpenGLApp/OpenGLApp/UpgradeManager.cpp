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

int UpgradeManager::getGenericCurrentValue(UpgradeIndex upgradeIndex)
{
    int intUpgradeIndex = static_cast<int>(upgradeIndex);
    return (intUpgradeIndex < 0 || intUpgradeIndex >= currentUpgrades.size()) ? -1 : currentUpgrades[intUpgradeIndex].currentValue;
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
    int intUpgradeIndex = static_cast<int>(upgradeIndex);
    if (intUpgradeIndex < 0 || intUpgradeIndex >= currentUpgrades.size())
        return;
    currentUpgrades[intUpgradeIndex].cost = currentUpgrades[intUpgradeIndex].cost * currentUpgrades[intUpgradeIndex].costIncrementRate;
}
