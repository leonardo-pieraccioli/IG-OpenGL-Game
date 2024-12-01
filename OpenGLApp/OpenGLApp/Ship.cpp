#include "Ship.h"

Ship::Ship(int nProjectiles)
{
	this->nProjectiles = nProjectiles;
}

void Ship::restoreHealth()
{
	currentHealth = maxHealth;
}
