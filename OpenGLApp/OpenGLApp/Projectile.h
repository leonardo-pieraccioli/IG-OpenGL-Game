#pragma once
#include "MiniEngine/GameObject.h"
#include "MiniEngine/UtilsF.h"

class Projectile : public GameObject {
public:
	Projectile(float damage = 10.0f, float speed = 12.5f, float destroyDistance = 15.f);

	void Update(float deltaTime) override;
	void Draw(Shader shader) override;
	void Move(std::pair<float, float> newCoords);

	float damage;

private:
	float speed;
	float destroyDistance;
};

