#pragma once
#include "GameObject.h"

class ShootingEntity : public GameObject
{
private:
	int health;
	int damage;
	float shootingRate;

public:
	ShootingEntity();

	void Move();
	void Shoot();
};

