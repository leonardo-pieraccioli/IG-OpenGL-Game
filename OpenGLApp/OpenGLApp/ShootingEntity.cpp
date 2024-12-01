#include "ShootingEntity.h"

ShootingEntity::ShootingEntity(int maxHealth, int damage, float shootingRate)
{
	this->maxHealth = maxHealth;
	this->currentHealth = maxHealth;
	this->damage = damage;
	this->shootingRate = shootingRate;
}

void ShootingEntity::Move()
{
	// da implementare
}

void ShootingEntity::Shoot()
{
	// da implementare
}
