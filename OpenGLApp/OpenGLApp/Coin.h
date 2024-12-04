#pragma once
#include "MiniEngine/GameObject.h"
#include "MiniEngine/Game.h"
#include <random>

// Range di "coordinate" (ad occhio) per l'istanziazione di Coins.
const float MIN_WIDTH = -12.0f;
const float MAX_WIDTH = 12.0f;
const float MIN_HEIGHT = -6.5f;
const float MAX_HEIGHT = 6.5f;


class Coin : public GameObject
{
private:
	int moneyAmount;

public:
	Coin(int initialAmount);

	void setMoney(int money);
	int getMoney();
	static void generateCoins(float deltaTime, Game& SpaceDefender);
};

