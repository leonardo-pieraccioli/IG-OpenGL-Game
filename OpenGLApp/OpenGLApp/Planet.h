#pragma once
#include "MiniEngine/GameObject.h"
#include "MiniEngine/Health.h"

#define INITIAL_HEALTH 100

class Planet : public GameObject
{
private:
	float rotationRate;
	
	void rotatePlanet(float deltaTime);
	void explode();
public:
	Health health = Health(INITIAL_HEALTH);

	Planet(float rotationRate = 20.f);

	void Update(float deltaTime) override;
	void Damage(int damage);
	void resetPlanet();
};