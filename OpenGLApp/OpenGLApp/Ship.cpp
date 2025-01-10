#include "Ship.h"
#include "MiniEngine/Game.h"

Ship::Ship(int nProjectiles)
{
	this->nProjectiles = nProjectiles;
}

void Ship::restoreHealth()
{
	currentHealth = maxHealth;
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

/*void Ship::Draw(Shader shader)
{
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    model = glm::rotate(model, glm::radians(this->transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
    model = glm::rotate(model, glm::radians(this->transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
    model = glm::rotate(model, glm::radians(this->transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
    model = glm::translate(model, this->transform.getPosition());
    model = glm::scale(model, this->transform.getScale());

    shader.SetMatrix4("model", model);
    objectModel.Draw(shader);
}*/