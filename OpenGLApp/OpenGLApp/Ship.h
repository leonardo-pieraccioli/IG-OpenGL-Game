#pragma once
#include "ShootingEntity.h"

class Ship : public ShootingEntity
{
private:
	int nProjectiles;

public:
	Ship(int nProjectiles = 1);

	void restoreHealth();
};

