#pragma once
#include "ShootingEntity.h"
#include "MiniEngine/UtilsF.h"

class Enemy : public ShootingEntity
{
private:
	int rewardMoney;
	int rewardScore;
	float speed;

public:
	Enemy(int rewardMoney = 100, int rewardScore = 100, float speed = 5.f);

	void Update(float deltaTime) override;
	void Draw(Shader shader) override;
	void Move(std::pair<float, float> newCoords);
};

