#pragma once
#include "GameObject.h"

class Planet : public GameObject
{
private:
	int maxHealth;
	int currentHealth;
	float rotationRate;

	void rotatePlanet(float deltaTime);
	void explode();
public:
	Planet(int maxHealth = 100, float rotationRate = 45.f);

	int getCurrentHealth();
	float getCurrentHealthRatio();
	void Update(float deltaTime) override;
	void damageActor(int damage);
};

