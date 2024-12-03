#pragma once
#include "ShootingEntity.h"

class Ship : public ShootingEntity
{
private:
	int nProjectiles;
	float movementRate = 90.0f;

public:
	Ship(int nProjectiles = 1);

	void Update(float deltaTime) override;
	void Draw(Shader ourShader) override;

	void restoreHealth();

	float getShipMovementRate();
};

