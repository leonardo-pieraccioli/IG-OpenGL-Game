#pragma once
#include "MiniEngine/GameObject.h"
#include "Ship.h"
#include <array>
#include "IObserver.h"
#include "TimerManager.h"

class Player : public GameObject, IObserver
{
private:
	static const int NUM_OF_SHIPS = 4;
	std::array<Ship, NUM_OF_SHIPS> shipArray;
	int money;
	float shootingRate = 0.5f;
	bool canShoot = true;
	float shipDistance = 2.25f;
	Timer* shootingTimer;
	// upgrade

public:
	Player();

	void Update(float deltaTime) override;
	void Draw(Shader shader) override;
	void moveHip(int direction, float deltaTime);
	int getMoney();
	void setMoney(int money);
	void addMoney(int moneyAmount);
	void setShootingRate(float shootingRate);
	float getShootingRate();
	void shootWithShips();

	// ----------------------------------
	// IObserver functions implementation
	void getNotified(std::string timerName, bool isCallbackEnabled) override;
};

