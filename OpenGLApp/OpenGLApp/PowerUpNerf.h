#pragma once
#include "Collectables.h"

const static glm::vec3 pwupnScale = glm::vec3(0.1f, 0.1f, 0.1f);
const static float modifierTimeAmount = 2.5f;

class PowerUpNerf : public Collectables
{
private:
	const float modifiedShootingRate = 0.5f;
	const float modifiedMovementRate = 0.5f;
	
public:
	PowerUpNerf(string type, float despawnTime, unsigned int texture);
	PowerUpNerf(string type, float despawnTime, float x, float y);

	bool doesPwUpNOverlap(glm::vec3 mouseWorldCoord);
	float getModifiedShootingRate();
	float getModifiedMovementRate();

	void Draw(Shader shader) override;
};

