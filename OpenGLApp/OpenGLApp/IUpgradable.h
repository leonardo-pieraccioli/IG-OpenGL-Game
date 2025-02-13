#pragma once
#include "UpgradeManager.h"

class IUpgradable {//: public UpgradeManager {
public:
	virtual void upgrade(UpgradeIndex upgradeIndex) = 0;
	virtual void resetUpgrades() = 0;
};