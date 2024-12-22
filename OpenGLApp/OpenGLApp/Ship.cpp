#include "Ship.h"

Ship::Ship(int nProjectiles)
{
	this->nProjectiles = nProjectiles;
}

void Ship::restoreHealth()
{
	currentHealth = maxHealth;
}

float Ship::getShipMovementRate()
{
    return movementRate;
}

void Ship::Update(float deltaTime)
{

}

void Ship::Draw(Shader shader)
{
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    model = glm::rotate(model, glm::radians(this->transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
    model = glm::rotate(model, glm::radians(this->transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
    model = glm::rotate(model, glm::radians(this->transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
    model = glm::translate(model, this->transform.getPosition());
    model = glm::scale(model, this->transform.getScale());

    shader.use();
    shader.setMat4("model", model);
    objectModel.Draw(shader);
}