#include "Coin.h"
#include <utility>
#include "MiniEngine/Game.h"

static bool loaded = false;
static Model coinModel;

Coin::Coin(int initialAmount, float despawnTime, unsigned int texture) : Collectables(despawnTime, texture)
{
	moneyAmount = initialAmount;
	this->despawnTime = despawnTime;
	tag = "Coin";
	initDestroyTimer();

	if (!loaded)
	{
		loaded = true;
		coinModel = Model("Assets/Models/Coin.obj");
	}
}

Coin::Coin(int initialAmount, float despawnTime, float x, float y) : Collectables(despawnTime, x, y)
{
	moneyAmount = initialAmount;
	this->despawnTime = despawnTime;
	tag = "Coin";
	initDestroyTimer();
	
	if (!loaded)
	{
		loaded = true;
		coinModel = Model("Assets/Models/Coin.obj");
	}
}

void Coin::Update(float deltaTime)
{
	Collectables::Update(deltaTime);
}

void Coin::Draw(Shader shader)
{
	// calculate the model matrix for each object and pass it to shader before drawing
	glm::mat4 model_mat = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model_mat = glm::translate(model_mat, transform.position);
	model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
	model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
	model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
	model_mat = glm::scale(model_mat, transform.getScale());
	shader.SetMatrix4("model", model_mat);
	coinModel.Draw(shader);
}

void Coin::setMoney(int money)
{
	moneyAmount = money;
}

int Coin::getMoney()
{
	return moneyAmount;
}

bool Coin::doesCoinOverlap(glm::vec3 mouseWorldCoord)
{
	return (mouseWorldCoord.x >= colliderCorners[0].x && mouseWorldCoord.x <= colliderCorners[1].x && mouseWorldCoord.y <= colliderCorners[0].y && mouseWorldCoord.y >= colliderCorners[1].y) ? true : false;
}