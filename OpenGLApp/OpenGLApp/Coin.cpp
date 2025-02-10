#include "Coin.h"
#include <utility>
#include "MiniEngine/Game.h"


Coin::Coin(int initialAmount, float despawnTime, unsigned int texture) : Collectables(despawnTime, texture)
{
	moneyAmount = initialAmount;
	this->despawnTime = despawnTime;
	tag = "Coin";
	initDestroyTimer();

	objectModel = Model("Assets/Models/Coin.obj");
}

Coin::Coin(int initialAmount, float despawnTime, float x, float y) : Collectables(despawnTime, x, y)
{
	moneyAmount = initialAmount;
	this->despawnTime = despawnTime;
	tag = "Coin";
	initDestroyTimer();

	objectModel = Model("Assets/Models/Coin.obj");
}

void Coin::Update(float deltaTime)
{
	Collectables::Update(deltaTime);
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