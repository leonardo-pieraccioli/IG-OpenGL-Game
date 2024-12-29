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

	objectModel = Model("Assets/Models/Planet.obj");
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

/*void Planet::Draw(Shader shader)
{
	// calculate the model matrix for each object and pass it to shader before drawing
	glm::mat4 model_mat = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model_mat = glm::translate(model_mat, transform.position);
	model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
	model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
	model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
	model_mat = glm::scale(model_mat, transform.getScale());
	shader.SetMatrix4("model", model_mat);
	objectModel.Draw(shader);
}*/

void Planet::damageActor(int damage)
{
	currentHealth -= damage;
	if (currentHealth <= 0) {
		explode();
	}
}

