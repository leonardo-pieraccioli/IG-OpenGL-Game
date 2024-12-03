#include "Ship.h"

Ship::Ship(int nProjectiles)
{
	this->nProjectiles = nProjectiles;
}

void Ship::restoreHealth()
{
	currentHealth = maxHealth;
}

void Ship::Update(float deltaTime)
{
	this->transform.rotation.z += movementRate * deltaTime;
}
