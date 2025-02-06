#pragma once
#include "ShootingEntity.h"
#include "Projectile.h"
#include "MiniEngine/UtilsF.h"


class Ship : public ShootingEntity
{
private:
	int nProjectiles;
	float movementRate = 90.0f;

public:
	Ship(int nProjectiles = 1);
	
	void Update(float deltaTime) override;
	//void Draw(Shader shader) override;
	void Shoot();
	void Damage(int damage);
	void Die() override;

	float getShipMovementRate();
};

