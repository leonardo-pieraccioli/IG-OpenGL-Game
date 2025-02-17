#include "PowerUpNerf.h"

static bool PUloaded = false;
static bool Nloaded = false;

static Model PUmodel;
static Model Nmodel;

PowerUpNerf::PowerUpNerf(string type, float despawnTime, unsigned int texture) : Collectables(despawnTime, texture)
{
	if (type == "PowerUp" || type == "Nerf") {
		this->despawnTime = despawnTime;
		tag = type;
		initDestroyTimer();
		
		if (type == "PowerUp" && !PUloaded)
		{
			PUmodel = Model("Assets/Models/powerup.obj");
			PUloaded = true;
		}
		else if (!Nloaded)
		{
			Nmodel = Model("Assets/Models/nerf.obj");
			Nloaded = true;
		}
	}
}

PowerUpNerf::PowerUpNerf(string type, float despawnTime, float x, float y) : Collectables(despawnTime, x, y)
{
	if (type == "PowerUp" || type == "Nerf") {
		this->despawnTime = despawnTime;
		tag = type;
		initDestroyTimer();

		if (type == "PowerUp" && !PUloaded)
		{
			PUmodel = Model("Assets/Models/powerup.obj");
			PUloaded = true;
		}
		else if (!Nloaded)
		{
			Nmodel = Model("Assets/Models/nerf.obj");
			Nloaded = true;
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

void PowerUpNerf::Draw(Shader shader)
{
	// calculate the model matrix for each object and pass it to shader before drawing
	glm::mat4 model_mat = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model_mat = glm::translate(model_mat, transform.position);
	model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
	model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
	model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
	model_mat = glm::scale(model_mat, transform.getScale());
	shader.SetMatrix4("model", model_mat);
	if (tag == "PowerUp")
	{
		PUmodel.Draw(shader);
	}
	else if (tag == "Nerf")
	{
		Nmodel.Draw(shader);
	}
}