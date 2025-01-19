#pragma once
#include "MiniEngine/GameObject.h"
#include <random>

// Range di "coordinate" per l'istanziazione di Coins.
const float MIN_WIDTH = -12.0f;
const float MAX_WIDTH = 12.0f;
const float MIN_HEIGHT = -6.5f;
const float MAX_HEIGHT = 6.5f;
const static glm::vec3 coinScale = glm::vec3(0.25f, 0.25f, 0.25f);


class Coin : public GameObject
{
private:
	int moneyAmount;
	glm::vec3 colliderCorners[2];

public:
	Coin(int initialAmount, unsigned int texture);
	Coin(int initialAmount, float x, float y);

	void Update(float deltaTime) override;

	void setMoney(int money);
	int getMoney();
	bool doesCoinOverlap(glm::vec3 mouseWorldCoord);
	static void generateCoins(float deltaTime);
};

