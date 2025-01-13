#pragma once
#include "MiniEngine/GameObject.h"
#include "MiniEngine/Health.h"

class ShootingEntity : public GameObject
{
protected:
	int damage;
	float shootingRate;

public:
	Health health = Health(0);
	ShootingEntity(int damage = 5, float shootingRate = 1.f);

	void Move();
	void Shoot();
};

