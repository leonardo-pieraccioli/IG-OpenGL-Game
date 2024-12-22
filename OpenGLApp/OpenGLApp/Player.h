#pragma once
#include "MiniEngine/GameObject.h"
#include "Ship.h"
#include <array>

class Player : public GameObject
{
private:
	static const int NUM_OF_SHIPS = 4;
	std::array<Ship, NUM_OF_SHIPS> shipArray;
	int money;
	// upgrade

public:
	Player();

	void Update(float deltaTime) override;
	void Draw(Shader shader) override;
	void moveHip(int direction, float deltaTime);
	int getMoney();
	void setMoney(int money);
	void addMoney(int moneyAmount);
};

