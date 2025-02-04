#include "Player.h"

Player::Player()
{
	glm::vec3 shipScale = glm::vec3(.35, .35, .35);
	objectModel = Model("Assets/Models/spaceship.obj");
	shipArray[0] = Ship();
	shipArray[0].transform = Transform(glm::vec3(shipDistance, 0.f, 0.0f), glm::vec3(0.f, 0.f, 0.f), shipScale);
	shipArray[0].objectModel = Model("Assets/Models/spaceship.obj");
	money = 0;
	shootingTimer = TimerManager::CreateTimer(1 / shootingRate, false, "PlayerShootingTimer", true, this);
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
		shipArray[i].transform.position = utilsF::rotateAroundZ(direction == 0 ? -(shipArray[i].getShipMovementRate() * deltaTime) : shipArray[i].getShipMovementRate() * deltaTime, shipArray[i].transform.rotation.z, shipDistance);
		shipArray[i].transform.rotation.z += direction == 0 ? -(shipArray[i].getShipMovementRate() * deltaTime) : shipArray[i].getShipMovementRate() * deltaTime;
		
	}
}

int Player::getMoney()
{
	return money;
}

void Player::setMoney(int money)
{
	this->money = money;
}

void Player::addMoney(int moneyAmount)
{
	money += moneyAmount;
}

int Player::getScore()
{
	return score;
}
void Player::setScore(int score)
{
	this->score = score;

}
void Player::addScore(int scoreAmount)
{
	score += scoreAmount;
}

void Player::setShootingRate(float shootingRate)
{
	this->shootingRate = shootingRate;
}

float Player::getShootingRate()
{
	return shootingRate;
}

void Player::shootWithShips()
{
	if(canShoot) {
		canShoot = false;
		shootingTimer->resetTimer(true);
		//for (auto ships : shipArray) {
		//	ships.Shoot();
		//}
		shipArray[0].Shoot();
	}
}

void Player::resetPlayer()
{
	shipArray[0].transform = Transform(glm::vec3(shipDistance, 0.f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(.35, .35, .35));
	for (int i = 1; i < shipArray.size(); i++) {
		shipArray[i].Die();
	}
	std::fill(std::next(std::begin(shipArray)), std::end(shipArray), 0);
	money = 0;
	score = 0;
	shootingRate = 1.0f;
	canShoot = true;
	shootingTimer->resetTimer(false);
}

void Player::getNotified(std::string timerName, bool isCallbackEnabled)
{
	canShoot = true;
}

GameObject* Player::CheckShipCollision(glm::vec3 position, float radius)
{
	for (int i = 0; i < shipArray.size(); i++) {

		if (glm::distance(shipArray[i].transform.position, position) < shipArray[i].transform.scale.x + radius)
		{
			return &shipArray[i];
		}
	}
	return nullptr;
}