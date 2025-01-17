#pragma once
#include "ShootingEntity.h"
#include "MiniEngine/UtilsF.h"
#include "MiniEngine/Game.h"
#include "Projectile.h"
#include <random>

class Enemy : public ShootingEntity, IObserver
{
private:
	int rewardMoney;
	int rewardScore;
	float speed;
	float shootingDistance;
	float shootingRate;
	bool canShoot = true;
	static int enemyID;
	Timer shootingTimer;

public:
	Enemy(int rewardMoney = 100, int rewardScore = 100, float speed = 5.f, float shootingDistance = 5.0f, float shootingRate = 0.2f);

	void Update(float deltaTime) override;
	void Draw(Shader shader) override;
	void Move(std::pair<float, float> newCoords);
	void Shoot();

	void getNotified(std::string timerName, bool isCallbackEnabled) override;
	static void generateEnemies(float deltaTime);
};

