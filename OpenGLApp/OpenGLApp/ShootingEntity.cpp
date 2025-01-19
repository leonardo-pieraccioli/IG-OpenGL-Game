#include "ShootingEntity.h"
#include "MiniEngine/Game.h"

ShootingEntity::ShootingEntity(int damage, float shootingRate)
{
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

void ShootingEntity::Die()
{
	Game::Instance().DestroyGameObject(this);
}
