#include "Planet.h"

void Planet::rotatePlanet(float deltaTime)
{
	this->transform.rotation.y += rotationRate * deltaTime;
}

void Planet::explode()
{
	//Esplosione? Game
}

Planet::Planet(int health, float rotationRate)
{
	this->maxHealth = (maxHealth >= -0.00001f && maxHealth <= 0.00001f ? 0.f : maxHealth);
	this->currentHealth = maxHealth;
	this->rotationRate = rotationRate;
}

int Planet::getCurrentHealth()
{
	return currentHealth;
}

float Planet::getCurrentHealthRatio()
{
	return currentHealth / maxHealth;
}

void Planet::Update(float deltaTime)
{
	rotatePlanet(deltaTime);
}

void Planet::damageActor(int damage)
{
	currentHealth -= damage;
	if (currentHealth <= 0) {
		explode();
	}
}

