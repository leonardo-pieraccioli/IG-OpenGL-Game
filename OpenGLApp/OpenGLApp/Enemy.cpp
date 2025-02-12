#include "Enemy.h"
#include "TimerManager.h"
#include "Coin.h"
#include "PowerUpNerf.h"

float currentsTime = 0.0f;
float timersActivation = 2.0f;
std::pair<float, float> generateEnemyCoordinates(float radius);
static glm::vec3 enemyScale = glm::vec3(0.25f, 0.25f, 0.25f);
glm::vec3 planetPosition(0.0f, 0.0f, 0.0f); // Planet position
static Model enemyModel;
static float enemySpeed;
static float actualSpeed;
static float enemyShootingRate; 
static float actualEnemyShootingRate;

Enemy::Enemy(int rewardMoney, int rewardScore, float shootingDistance, float shootingRate, float decelerationDistance)
{
	this->rewardMoney = rewardMoney;
	this->rewardScore = rewardScore;
	this->shootingDistance = shootingDistance;
	this->decelerationDistance = decelerationDistance;
	enemyShootingRate = shootingRate == 0.f ? 0.000001f : shootingRate;
	this->tag = "Enemy";
	actualSpeed = enemySpeed = utilsF::randomNumberInInterval(baseMinSpeed * (Game::Instance().getRound() / 10) + 2.25, baseMaxSpeed * (Game::Instance().getRound() / 10) + 6.75);
	actualEnemyShootingRate = shootingRate;
	shootingTimer = TimerManager::CreateTimer(1 / (shootingRate * Game::Instance().getRound()), false, "Enemy" + std::to_string(this->GetID()), true, this);
	health.UpgradeMax(Game::Instance().getRound() * baseHealth);
	timersActivation = utilsF::randomNumberInInterval(baseMinSpawnRate / std::log2(Game::Instance().getRound() + 1), baseMaxSpawnRate / std::log2(Game::Instance().getRound() + 1));
	srand((unsigned)time(NULL));
}

void Enemy::Init(Model model)
{
	enemyModel = Model("Assets/Models/enemy1.obj");
}

void Enemy::Update(float deltaTime)
{
	float a_x = this->transform.position.x;
	float a_y = this->transform.position.y;

	float distanceFromCenter = utilsF::distance2DSquare(a_x, a_y, 0.0f, 0.0f);

	if (!shouldStop && distanceFromCenter > shootingDistance && distanceFromCenter <= decelerationDistance) {
		tDeceleration = utilsF::interpolateOnRadiuses(a_x, a_y, this->transform.rotation.z, shootingDistance, decelerationDistance);//a_x, a_y, ship_z, minR, maxR
	}
	if (!shouldStop && distanceFromCenter > shootingDistance) {
		Move(utilsF::calculateForwardXY(this->transform.rotation.z, deltaTime, this->transform.position.x, this->transform.position.y, enemySpeed*tDeceleration));
	}
	else {
		if (!shootingTimer->getIsTicking()) {
			shootingTimer->resetTimer(true);
			shouldStop = true;
		}
		if (canShoot && frameCounter == 0) {
			canShoot = false;
			Shoot();
		}
		switch (frameCounter) {
			case 0:
				tVibration = 1 - (shootingTimer->getRemainingTime() * enemyShootingRate);
				currentVibrationCoords = utilsF::generateVibrationCoords(tVibration * maxVibrationRadius);
				Move(std::pair<float, float>(currentVibrationCoords.first + a_x, currentVibrationCoords.second + a_y));
				break;
			case 1:
				Move(std::pair<float, float>(-currentVibrationCoords.first + a_x, -currentVibrationCoords.second + a_y));
				break;
			case 2:
				Move(std::pair<float, float>(-currentVibrationCoords.first + a_x, -currentVibrationCoords.second + a_y));
				break;
			case 3:
				Move(std::pair<float, float>(currentVibrationCoords.first + a_x, currentVibrationCoords.second + a_y));
				frameCounter = -1;
				break;
		}
		frameCounter++;
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
	enemyModel.Draw(shader);
}

void Enemy::Move(std::pair<float, float> newCoords)
{
	this->transform.position.x = newCoords.first;
	this->transform.position.y = newCoords.second;
}

void Enemy::Shoot()
{
	float damage = baseDamage * Game::Instance().getRound();
	ShootingEntity::Shoot();
	std::pair<float, float> pCoords = utilsF::calculateForwardXY(this->transform.rotation.z, 1.0f, this->transform.position.x, this->transform.position.y, 0.8f);
	Game::Instance().InstantiateGameObject(new Projectile(damage), new Transform(glm::vec3(pCoords.first, pCoords.second, 0.0f), glm::vec3(this->transform.rotation.x, this->transform.rotation.y, this->transform.rotation.z), glm::vec3(0.10f, 0.25f, 0.25f)));
	shootingTimer->resetTimer(1/ (enemyShootingRate * Game::Instance().getRound()), true, true);
}

void Enemy::Die()
{
	int random = rand() % 7 + 1;
	std::string path = "Assets/Sounds/explosion/explosion" + to_string(random) + ".mp3";
	SoundManager::Instance().playSound(path.c_str(), false);
	Game::Instance().player->addScore(rewardScore);
	auto x = transform.position.x;
	auto y = transform.position.y;
	if (random > 0 && random < 6) {
		Game::Instance().InstantiateGameObject(new Coin(rewardMoney, 5.0f, x, y), new Transform(glm::vec3(x, y, -2.0f), glm::vec3(90.f, 0.f, 0.f), coinScale));
	}
	else if (random == 6) {
		Game::Instance().InstantiateGameObject(new PowerUpNerf("PowerUp", 5.0f, x, y), new Transform(glm::vec3(x, y, -2.0f), glm::vec3(90.f, 0.f, 0.f), pwupnScale));
	}
	else if (random == 7) {
		Game::Instance().InstantiateGameObject(new PowerUpNerf("Nerf", 5.0f, x, y), new Transform(glm::vec3(x, y, -2.0f), glm::vec3(270.f, 0.f, 0.f), pwupnScale));
	}
	Game::Instance().DestroyGameObject(this);
}

float Enemy::getActualSpeed()
{
	return actualSpeed;
}

void Enemy::setActualSpeed(float newSpeed)
{
	actualSpeed = newSpeed;
}

void Enemy::Nerf(bool nerf, float srNerfAmount, float mrNerfAmount)
{
	if (nerf && enemySpeed == actualSpeed)
	{
		enemySpeed *= mrNerfAmount;
		enemyShootingRate *= srNerfAmount;
	}
	else if (enemySpeed < actualSpeed)
	{
		enemySpeed = actualSpeed;
		enemyShootingRate = actualEnemyShootingRate;
	}
}

void Enemy::getNotified(std::string timerName, bool isCallbackEnabled)
{
	canShoot = true;
}

void Enemy::playChargeSound()
{
	std::string path = "Assets/Sounds/chargingShot.mp3";
	chargeSound = SoundManager::Instance().playSoundWithRetP(path.c_str(), true);
}

void Enemy::generateEnemies(float deltaTime)
{
	// Timer per gestire l'istanza dei nemici nel tempo
	currentsTime += deltaTime;
	if (currentsTime >= timersActivation) {
		currentsTime = 0;

		// Raggio della circonferenza in cui spawna il nemico
		float spawnRadius = 15.0f;

		std::pair<float, float> coordinates = generateEnemyCoordinates(spawnRadius);
		float x = coordinates.first;
		float y = coordinates.second;
		glm::vec3 objCoordinates(coordinates.first, coordinates.second, 0.0f); // Coordinate scelte del nemico

		// Calcola la direzione dal punto dell'oggetto al punto fisso
		glm::vec3 direction = glm::normalize(planetPosition - objCoordinates);

		// Calcola l'angolo di rotazione (in questo caso sul piano XY)
		float angle = atan2(direction.y, direction.x); // Angolo in radianti
		float angleInDegrees = glm::degrees(angle); // Angolo in gradi

		// Definisce la rotazione sull'asse Z
		glm::vec3 eulerAngles(0.0f, 0.0f, angleInDegrees);

		// Instanzia l'oggetto con la rotazione calcolata
		Transform *newTransform = new Transform(glm::vec3(x, y, 0), eulerAngles, enemyScale);
		newTransform->collisionRadius = .5f;
		Game::Instance().InstantiateGameObject(new Enemy(), newTransform);
	}
}

// Genera coordinate valide per l'apparizione dei nemici
std::pair<float, float> generateEnemyCoordinates(float radius) {
	float angle = utilsF::randomNumberInInterval(0.0f, 2.0f * 3.14);

	// Calcola le coordinate x e y lungo la circonferenza
	float x = radius * cos(angle);
	float y = radius * sin(angle);

	return std::make_pair(x, y);
}