#pragma once
#include "GameObject.h"

class Planet : GameObject
{
private:
	int maxHealth;
	int currentHealth;
	float rotationRate;

	void rotatePlanet(float deltaTime);
public:
	Planet(int maxHealth = 100, float rotationRate = 45.f);

	int getCurrentHealth();
	float getCurrentHealthRatio();
	void Update(float deltaTime) override;
};

