#pragma once
#include "ShootingEntity.h"
#include "Projectile.h"
#include "MiniEngine/UtilsF.h"
#include "IUpgradable.h"


class Ship : public ShootingEntity, public IUpgradable
{
private:
	int nProjectiles;
	float movementRate = 95.0f;

	const float maxPitchRotation = 45.0f;
	const float pitchRotationTime = 0.25f;
	float tLerp = 0.0f;

public:
	Ship(int nProjectiles = 1);
	
	void Update(float deltaTime) override;
	void Draw(Shader shader) override;
	void Shoot();
	void Damage(int damage);
	void Die() override;

	static float getActualMovementRate();
	static void setActualMovementRate(float movementRate);

	float getShipMovementRate();
	void setShipMovementRate(float movementRate);
	void updateTLerp(float deltaTime, int pitchRotVal);

	void upgrade(UpgradeIndex upgradeIndex) override;
	void resetUpgrades() override;

	static void setShipModel();
};

