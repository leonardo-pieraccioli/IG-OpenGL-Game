#pragma once
#include "ShootingEntity.h"

class Enemy : public ShootingEntity
{
private:
	int rewardMoney;
	int rewardScore;
	float speed;

public:
	Enemy(int rewardMoney = 100, int rewardScore = 100, float speed = 5.f);
};

