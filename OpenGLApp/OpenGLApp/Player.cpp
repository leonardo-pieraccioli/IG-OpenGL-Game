#include "Player.h"
#include "MiniEngine/Game.h"
#include "MiniEngine/SoundManager.h"

Player::Player()
{
	objectModel = Model("Assets/Models/spaceship.obj");
	ShipSetup();
	money = 0;
	this->tag = "Player";
	shootingTimer = TimerManager::CreateTimer(1 / shootingRate, false, "PlayerShootingTimer", true, this);
	srand((unsigned)time(NULL));
}

void Player::ShipSetup()
{
	glm::vec3 shipScale = glm::vec3(.35, .35, .35);

	shipArray[0] = new Ship();
	shipArray[0]->transform = Transform(glm::vec3(shipDistance, 0.f, 0.0f), glm::vec3(0.f, 0.f, 0.f), shipScale);
	shipArray[0]->objectModel = objectModel;
	shipArray[0]->isActive = true;

	shipArray[1] = new Ship();
	shipArray[1]->transform = Transform(glm::vec3(-shipDistance, 0.f, 0.0f), glm::vec3(0.f, 0.f, 180.f), shipScale);
	shipArray[1]->objectModel = objectModel;
	shipArray[1]->isActive = false;

	shipArray[2] = new Ship();
	shipArray[2]->transform = Transform(glm::vec3(0.0f, shipDistance, 0.0f), glm::vec3(0.f, 0.f, 90.f), shipScale);
	shipArray[2]->objectModel = objectModel;
	shipArray[2]->isActive = false;

	shipArray[3] = new Ship();
	shipArray[3]->transform = Transform(glm::vec3(0.f, -shipDistance, 0.0f), glm::vec3(0.f, 0.f, -90.f), shipScale);
	shipArray[3]->objectModel = objectModel;
	shipArray[3]->isActive = false;
}

void Player::Update(float deltaTime)
{
	bool isOver = true;
	for (auto ship : shipArray) {
		if (ship->isActive)
		{
			//ship.Update(deltaTime);
			ship->updateTLerp(deltaTime, pitchRotationValue);
			isOver = false;
		}
	}
	if (isOver)
	{
		Game::Instance().ChangeGameState(GameState::GameOver);
	}
}

void Player::Draw(Shader shader)
{
	for (auto ship : shipArray) {
		if (ship->isActive) {
			ship->Draw(shader);
		}
	}
}

void Player::moveHip(int direction, float deltaTime)
{
	for (int i = 0; i < shipArray.size(); i++) {
		shipArray[i]->transform.position = utilsF::rotateAroundZ(direction == 0 ? -(shipArray[i]->getShipMovementRate() * deltaTime) : shipArray[i]->getShipMovementRate() * deltaTime, shipArray[i]->transform.rotation.z, shipDistance);
		shipArray[i]->transform.rotation.z += direction == 0 ? -(shipArray[i]->getShipMovementRate() * deltaTime) : shipArray[i]->getShipMovementRate() * deltaTime;
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
	if (moneyAmount < 0)
	{
		SoundManager::Instance().playSound("Assets/Sounds/purchase.mp3", false);
	}
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
		playShootSound();
		for (auto ship : shipArray) 
		{
			if (ship->isActive) ship->Shoot();
		}
	}
}

void Player::resetPlayer()
{
	ShipSetup();
	money = 0;
	score = 0;
	shootingRate = 1.0f;
	canShoot = true;
	shootingTimer->resetTimer(false);
}

void Player::setPitchRotationValue(int pitchRotationValue)
{
	this->pitchRotationValue = pitchRotationValue;
}

void Player::playShootSound()
{
	int random = rand() % 3 + 1;
	std::string path = "Assets/Sounds/blast/blast" + to_string(random) + ".mp3";
	SoundManager::Instance().playSound(path.c_str(), false);
}

void Player::getNotified(std::string timerName, bool isCallbackEnabled)
{
	canShoot = true;
}

GameObject* Player::CheckShipCollision(glm::vec3 position, float radius)
{
	for (int i = 0; i < shipArray.size(); i++) {

		if (shipArray[i]->isActive && glm::distance(shipArray[i]->transform.position, position) < shipArray[i]->transform.collisionRadius + radius)
		{
			return shipArray[i];
		}
	}
	return nullptr;
}