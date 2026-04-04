#pragma once
#include "ShootingEntity.h"
#include "MiniEngine/UtilsF.h"
#include "MiniEngine/Game.h"
#include "Projectile.h"
#include <random>

class Enemy : public ShootingEntity, IObserver
{
private:
	// constants
	const float maxVibrationRadius = 0.05f;
	const float baseDamage = 1.0f;
	const float baseHealth = 2.5f;
	const float baseMinSpawnRate = 1.f;
	const float baseMaxSpawnRate = 1.2f;
	const float baseMinSpeed = 2.2f;
	const float baseMaxSpeed = 3.0f;

	float enemySpeed;
	int rewardMoney;
	int rewardScore;
	float shootingDistance;
	float decelerationDistance;
	bool canShoot = false;
	Timer* shootingTimer;
	float tDeceleration = 1.0f;
	float tVibration = 0.0f;
	int frameCounter = 0;
	
	std::pair<float, float> currentVibrationCoords;
	bool shouldStop = false;


public:
	Enemy(int rewardMoney = 100, int rewardScore = 100, float shootingDistance = 4.5f, float shootingRate = 2.f, float decelerationDistance = 7.0f);

	static void Init(Model model);
	void Update(float deltaTime) override;
	void Draw(Shader shader) override;
	void Move(std::pair<float, float> newCoords);
	void Shoot();
	void Die() override;

	static float getActualSpeed();
	static void setActualSpeed(float newSpeed);
	static int getCurrentEnemyCount();
	static void setCurrentEnemyCount(int newCount);
	// static void Nerf(bool nerf, float srNerfAmount=1, float mrNerfAmount=1);

	void getNotified(std::string timerName, bool isCallbackEnabled) override;
	static void generateEnemies(float deltaTime);
};

