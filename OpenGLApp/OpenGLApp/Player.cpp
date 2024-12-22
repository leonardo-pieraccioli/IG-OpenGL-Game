#include "Player.h"

Player::Player()
{
	glm::vec3 shipScale = glm::vec3(.35, .35, .35);

	shipArray[0] = Ship();
	shipArray[0].transform = Transform(glm::vec3(2.25f, 0.f, 0.0f), glm::vec3(0.f, 0.f, 0.f), shipScale);
	shipArray[0].objectModel = Model("Assets/Models/spaceship.obj");
	money = 0;

}

void Player::Update(float deltaTime)
{
	for (int i = 0; i < shipArray.size(); i++) {
		shipArray[i].Update(deltaTime);
	}
}

void Player::Draw(Shader shader)
{
	for (int i = 0; i < shipArray.size(); i++) {
		shipArray[i].Draw(shader);
	}
}

void Player::moveHip(int direction, float deltaTime)
{
	for (int i = 0; i < shipArray.size(); i++) {
		shipArray[i].transform.rotation.z += direction == 0 ? -(shipArray[i].getShipMovementRate() * deltaTime) : shipArray[i].getShipMovementRate() * deltaTime;
	}
}