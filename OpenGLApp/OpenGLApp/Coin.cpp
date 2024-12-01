#include "Coin.h"

Coin::Coin(int initialAmount)
{
	moneyAmount = initialAmount;
}

void Coin::setMoney(int money)
{
	moneyAmount = money;
}

int Coin::getMoney()
{
	return moneyAmount;
}
