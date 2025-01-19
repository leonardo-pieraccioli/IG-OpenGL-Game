#include "Enemy.h"
#include "TimerManager.h"
#include "Coin.h"

Enemy::Enemy(int rewardMoney, int rewardScore, float speed, float shootingDistance, float shootingRate)
{
	this->rewardMoney = rewardMoney;
	this->rewardScore = rewardScore;
	this->speed = speed;
	this->shootingDistance = shootingDistance;
	this->shootingRate = shootingRate == 0.f ? 0.000001f : shootingRate;

	this->health.UpgradeMax(20);

	objectModel = Model("Assets/Models/enemy1.obj");
}

void Enemy::Update(float deltaTime)
{
	float a_x = this->transform.position.x;
	float a_y = this->transform.position.y;

	if (utilsF::distance2DSquare(a_x, a_y, 0.0f, 0.0f) > shootingDistance) {
		Move(utilsF::calculateForwardXY(this->transform.rotation.z, deltaTime, this->transform.position.x, this->transform.position.y, speed));
	}
	else {
		if (canShoot) {
			canShoot = false;
			Shoot();
		}
	}
}

void Enemy::Draw(Shader shader)
{
	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, this->transform.getPosition());
	model = glm::rotate(model, glm::radians(this->transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
	model = glm::rotate(model, glm::radians(this->transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
	model = glm::rotate(model, glm::radians(this->transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
	
	model = glm::scale(model, this->transform.getScale());

	shader.SetMatrix4("model", model);
	objectModel.Draw(shader);
}

void Enemy::Move(std::pair<float, float> newCoords)
{
	this->transform.position.x = newCoords.first;
	this->transform.position.y = newCoords.second;
}

void Enemy::Shoot()
{
	std::pair<float, float> pCoords = utilsF::calculateForwardXY(this->transform.rotation.z, 1.0f, this->transform.position.x, this->transform.position.y, 0.8f);
	Game::Instance().InstantiateGameObject(new Projectile(), new Transform(glm::vec3(pCoords.first, pCoords.second, 0.0f), glm::vec3(this->transform.rotation.x, this->transform.rotation.y, this->transform.rotation.z), glm::vec3(0.10f, 0.25f, 0.25f)));
	std::string timerName = "EnemyTimer" + std::to_string(this->GetID());
	TimerManager::CreateTimer(1 / shootingRate, true, timerName, true, this);
}

void Enemy::Die()
{
	Game::Instance().player->addScore(rewardScore);
	auto x = transform.position.x;
	auto y = transform.position.y;
	Game::Instance().InstantiateGameObject(new Coin(rewardMoney, x, y), new Transform(glm::vec3(x, y, -2.0f), glm::vec3(90.f, 0.f, 0.f), coinScale));
	Game::Instance().DestroyGameObject(this);
}

void Enemy::getNotified(std::string timerName, bool isCallbackEnabled)
{
	canShoot = true;
}
