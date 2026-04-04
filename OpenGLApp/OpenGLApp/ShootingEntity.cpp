#include "ShootingEntity.h"
#include "MiniEngine/Game.h"

ShootingEntity::ShootingEntity(int damage, float shootingRate)
{
	this->damage = damage;
	this->shootingRate = shootingRate;
	srand((unsigned)time(NULL));
}

void ShootingEntity::Move()
{
	// da implementare
}

void ShootingEntity::Shoot()
{
	int random = rand()%3 + 1;
	std::string path = "Assets/Sounds/blast/blast" + to_string(random) + ".mp3";
	SoundManager::Instance().playSound(path.c_str(), false);
}

void ShootingEntity::Die()
{
	Game::Instance().DestroyGameObject(this);
}
