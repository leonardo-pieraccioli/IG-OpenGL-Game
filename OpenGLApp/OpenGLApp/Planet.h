#pragma once
#include "MiniEngine/GameObject.h"
#include "MiniEngine/Health.h"
#include "IUpgradable.h"

#define INITIAL_HEALTH 100

class Planet : public GameObject, public IUpgradable
{
private:
	float rotationRate;
	
	void rotatePlanet(float deltaTime);
	void explode();
public:
	Health health = Health(INITIAL_HEALTH);

	Planet(float rotationRate = 20.f);

	void Update(float deltaTime) override;
	void Damage(int damage);
	void resetPlanet();

	// ------------------------------------
	// IUpgradable functions implementation
	void upgrade(UpgradeIndex upgradeIndex) override;
	void resetUpgrades() override;
};