#include "Projectile.h"
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
		//Game::Instance().DestroyGameObject(this); -----------------------------------> da fixare destroyGameObject, al momento l'implementazione causa problemi perchè non si può eliminare un elemento mentre si sta usando l'iteratore
	}
	Move(utilsF::calculateForwardXY(this->transform.rotation.z, deltaTime, this->transform.position.x, this->transform.position.y, speed));
}

//void Projectile::Draw(Shader shader)
//{
//}

void Projectile::Move(std::pair<float, float> newCoords)
{
	this->transform.position.x = newCoords.first;
	this->transform.position.y = newCoords.second;
}