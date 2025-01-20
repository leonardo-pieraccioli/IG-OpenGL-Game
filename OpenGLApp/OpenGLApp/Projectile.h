#pragma once
#include "MiniEngine/GameObject.h"
#include "MiniEngine/UtilsF.h"

class Projectile : public GameObject {
public:
	Projectile(float speed = 10.0f, float destroyDistance = 30.f);

	void Update(float deltaTime) override;
	//void Draw(Shader shader) override;
	void Move(std::pair<float, float> newCoords);

private:
	float speed;
	float destroyDistance;
};

