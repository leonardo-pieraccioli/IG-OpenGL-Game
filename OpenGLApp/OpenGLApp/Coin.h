#pragma once
#include "GameObject.h"

class Coin : public GameObject
{
private:
	int moneyAmount;

public:
	Coin(int initialAmount);

	void setMoney(int money);
	int getMoney();
};

