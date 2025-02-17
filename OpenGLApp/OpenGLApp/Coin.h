#pragma once
#include "Collectables.h"

const static glm::vec3 coinScale = glm::vec3(0.25f, 0.25f, 0.25f);

class Coin : public Collectables
{
private:
	int moneyAmount;

public:
	Coin(int initialAmount, float despawnTime, unsigned int texture);
	Coin(int initialAmount, float despawnTime, float x, float y);

	void Update(float deltaTime) override;
	void Draw(Shader shader) override;

	void setMoney(int money);
	int getMoney();
	bool doesCoinOverlap(glm::vec3 mouseWorldCoord);
};

