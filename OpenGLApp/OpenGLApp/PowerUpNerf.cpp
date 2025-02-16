#include "PowerUpNerf.h"

PowerUpNerf::PowerUpNerf(string type, float despawnTime, unsigned int texture) : Collectables(despawnTime, texture)
{
	if (type == "PowerUp" || type == "Nerf") {
		this->despawnTime = despawnTime;
		tag = type;
		initDestroyTimer();

		if (type == "PowerUp") {
			objectModel = Model("Assets/Models/RocketGreen.obj");
		}
		else {
			objectModel = Model("Assets/Models/Rocket.obj");
		}
	}
}

PowerUpNerf::PowerUpNerf(string type, float despawnTime, float x, float y) : Collectables(despawnTime, x, y)
{
	if (type == "PowerUp" || type == "Nerf") {
		this->despawnTime = despawnTime;
		tag = type;
		initDestroyTimer();

		if (type == "PowerUp") {
			objectModel = Model("Assets/Models/powerup.obj");
		}
		else {
			objectModel = Model("Assets/Models/nerf.obj");
		}
	}
}

bool PowerUpNerf::doesPwUpNOverlap(glm::vec3 mouseWorldCoord)
{
	return (mouseWorldCoord.x >= colliderCorners[0].x && mouseWorldCoord.x <= colliderCorners[1].x && mouseWorldCoord.y <= colliderCorners[0].y && mouseWorldCoord.y >= colliderCorners[1].y) ? true : false;
}

float PowerUpNerf::getModifiedShootingRate()
{
	return modifiedShootingRate;
}

float PowerUpNerf::getModifiedMovementRate()
{
	return modifiedMovementRate;
}
