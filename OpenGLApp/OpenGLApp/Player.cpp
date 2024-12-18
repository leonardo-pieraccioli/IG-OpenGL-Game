#include "Player.h"

Player::Player()
{
	glm::vec3 shipScale = glm::vec3(1, 1, 1);

	shipArray[0] = Ship();
	shipArray[0].transform = Transform(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), shipScale);
	money = 0;
}

void Player::Draw(Shader ourShader)
{
	for (int i = 0; i < shipArray.size(); i++) {
		shipArray[i].Draw(ourShader);
	}
}

void Player::moveHip(int direction, float deltaTime)
{
	for (int i = 0; i < shipArray.size(); i++) {
		shipArray[i].transform.rotation.z += direction == 0 ? -(shipArray[i].getShipMovementRate() * deltaTime) : shipArray[i].getShipMovementRate() * deltaTime;
	}
	
}

void Player::SetTextures(unsigned int texture1, unsigned int texture2)
{
	shipArray[0].SetTextures(texture1, texture2);
}
