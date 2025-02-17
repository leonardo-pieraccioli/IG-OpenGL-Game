#pragma once
#include "MiniEngine/GameObject.h"
#include "Ship.h"
#include <array>
#include "IObserver.h"
#include "TimerManager.h"

class Player : public GameObject, IObserver, IUpgradable
{
private:
	static const int NUM_OF_SHIPS = 4;
	int nextShipToActivate = 1;
	int money;
	int score;
	int pitchRotationValue = 0;
	float shootingRate = 1.0f;
	float actualShootingRate;
	bool canShoot = true;
	float shipDistance = 2.25f;
	Timer* shootingTimer;
	void ShipSetup();
	// upgrade

public:
	Player();

	std::array<Ship*, NUM_OF_SHIPS> shipArray;
	void Update(float deltaTime) override;
	void Draw(Shader shader) override;
	void moveHip(int direction, float deltaTime);
	void Nerf(bool nerf, float speedModification=1.f, float shootingRateModification=1.f);
	int getMoney();
	void setMoney(int money);
	void addMoney(int moneyAmount);
	int getScore();
	void setScore(int score);
	void addScore(int scoreAmount);
	void setShootingRate(float shootingRate);
	float getShootingRate();
	void shootWithShips();
	void resetPlayer();
	void setPitchRotationValue(int pitchRotationValue);
	void playShootSound();

	GameObject* CheckShipCollision(glm::vec3 position, float radius);

	// ----------------------------------
	// IObserver functions implementation
	void getNotified(std::string timerName, bool isCallbackEnabled) override;

	// ------------------------------------
	// IUpgradable functions implementation
	void upgrade(UpgradeIndex upgradeIndex) override;
	void resetUpgrades() override;
};

