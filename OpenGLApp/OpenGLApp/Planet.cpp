#include "Planet.h"

void Planet::rotatePlanet(float deltaTime)
{
	this->transform.rotation.y += rotationRate * deltaTime;
}

void Planet::explode()
{
	//Explosion, Game Over
}

Planet::Planet(float rotationRate)
{
	this->rotationRate = rotationRate;
	objectModel = Model("Assets/Models/Planet.obj");
}

void Planet::Update(float deltaTime)
{
	rotatePlanet(deltaTime);
}

void Planet::Damage(int damage)
{
	if (health.Damage(damage) <= 0) {
		explode();
	}
}

