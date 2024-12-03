#pragma once
#include "ShootingEntity.h"

class Ship : public ShootingEntity
{
private:
	int nProjectiles;
	float movementRate = 45.0f;

public:
	Ship(int nProjectiles = 1);

	void restoreHealth();
	void Update(float deltaTime) override;
};

