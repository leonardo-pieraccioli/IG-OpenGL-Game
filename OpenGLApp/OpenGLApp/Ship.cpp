#include "Ship.h"
#include "MiniEngine/Game.h"

#define SHIP_INITIAL_HEALTH 50

Ship::Ship(int nProjectiles)
{
	this->nProjectiles = nProjectiles;
    this->tag = "Ship";
    this->health.UpgradeMax(SHIP_INITIAL_HEALTH);
}

void Ship::Shoot()
{
    std::pair<float, float> pCoords = utilsF::calculateForwardXY(this->transform.rotation.z, 1.0f, this->transform.position.x, this->transform.position.y, 0.8f);
    Game::Instance().InstantiateGameObject(new Projectile(), new Transform(glm::vec3(pCoords.first, pCoords.second, 0.0f), glm::vec3(this->transform.rotation.x, this->transform.rotation.y, this->transform.rotation.z), glm::vec3(0.10f, 0.25f, 0.25f)));
}

float Ship::getShipMovementRate()
{
    return movementRate;
}

void Ship::Update(float deltaTime)
{

}