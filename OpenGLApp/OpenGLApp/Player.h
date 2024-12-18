#pragma once
#include "MiniEngine/GameObject.h"
#include "Ship.h"
#include <array>

class Player : public GameObject
{
private:
	static const int NUM_OF_SHIPS = 4;
	std::array<Ship, NUM_OF_SHIPS> shipArray;
	int money;
	// upgrade

public:
	Player();

	void Draw(Shader ourShader);
	void moveHip(int direction, float deltaTime);

	void SetTextures(unsigned int texture1, unsigned int texture2) override;
};

