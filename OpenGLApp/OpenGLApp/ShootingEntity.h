#pragma once
#include "MiniEngine/GameObject.h"

class ShootingEntity : public GameObject
{
protected:
	int maxHealth;
	int currentHealth;
	int damage;
	float shootingRate;

public:
	ShootingEntity(int maxHealth = 100, int damage = 5, float shootingRate = 1.f);

	void Move();
	void Shoot();
};

