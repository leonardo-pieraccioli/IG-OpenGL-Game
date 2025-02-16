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
	const float baseDamage = 10.0f;
	const float baseHealth = 5.0f;
	const float baseMinSpawnRate = 1.5f;
	const float baseMaxSpawnRate = 2.5f;
	const float baseMinSpeed = 5.5f;
	const float baseMaxSpeed = 5.5f;

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
	irrklang::ISound* chargeSound;

	void playChargeSound();

public:
	Enemy(int rewardMoney = 100, int rewardScore = 100, float shootingDistance = 5.0f, float shootingRate = 0.2f, float decelerationDistance = 7.0f);

	static void Init(Model model);
	void Update(float deltaTime) override;
	void Draw(Shader shader) override;
	void Move(std::pair<float, float> newCoords);
	void Shoot();
	void Die() override;

	static float getActualSpeed();
	static void setActualSpeed(float newSpeed);
	// static void Nerf(bool nerf, float srNerfAmount=1, float mrNerfAmount=1);

	void getNotified(std::string timerName, bool isCallbackEnabled) override;
	static void generateEnemies(float deltaTime);
};

