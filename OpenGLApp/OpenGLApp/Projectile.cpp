#include "Projectile.h"
#include "Enemy.h"
#include "MiniEngine/Game.h"

Projectile::Projectile(float speed, float destroyDistance)
{
	this->speed = speed;
	this->destroyDistance = destroyDistance;

	objectModel = Model("Assets/Models/laserbeam.obj");
}

void Projectile::Update(float deltaTime)
{
	float a_x = this->transform.position.x;
	float a_y = this->transform.position.y;

  	if (utilsF::distance2DSquare(a_x, a_y, 0.0f, 0.0f) > destroyDistance) {
		Game::Instance().DestroyGameObject(this);
	}
	Move(utilsF::calculateForwardXY(this->transform.rotation.z, deltaTime, this->transform.position.x, this->transform.position.y, speed));

	GameObject* hit = Game::Instance().CheckCollision(*this, this->transform.position, this->transform.scale);
	if(hit != nullptr)
	{
		ShootingEntity* shootingEntityHit = dynamic_cast<ShootingEntity*>(hit);
		if (shootingEntityHit != nullptr)
		{
			if (shootingEntityHit->health.Damage(10) <= 0)
			{
				shootingEntityHit->Die();
			}
		}
		Game::Instance().DestroyGameObject(this);
	}
}

//void Projectile::Draw(Shader shader)
//{
//}

void Projectile::Move(std::pair<float, float> newCoords)
{
	this->transform.position.x = newCoords.first;
	this->transform.position.y = newCoords.second;
}