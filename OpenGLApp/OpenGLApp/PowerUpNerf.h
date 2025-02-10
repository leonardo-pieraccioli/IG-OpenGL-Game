#pragma once
#include "Collectables.h"

const static glm::vec3 pwupnScale = glm::vec3(0.25f, 0.25f, 0.25f);
const static float modifierTimeAmount = 5.f;

class PowerUpNerf : public Collectables
{
private:
	const float modifiedShootingRate = 0.66f;
	const float modifiedMovementRate = 0.66f;
	
public:
	PowerUpNerf(string type, float despawnTime, unsigned int texture);
	PowerUpNerf(string type, float despawnTime, float x, float y);

	bool doesPwUpNOverlap(glm::vec3 mouseWorldCoord);
	float getModifiedShootingRate();
	float getModifiedMovementRate();
};

