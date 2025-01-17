#include "Enemy.h"
#include "TimerManager.h"

std::pair<float, float> generateEnemyCoordinates(float radius);
static glm::vec3 enemyScale = glm::vec3(0.25f, 0.25f, 0.25f);
glm::vec3 planetPosition(0.0f, 0.0f, 0.0f); // Planet position

int Enemy::enemyID = -1;

Enemy::Enemy(int rewardMoney, int rewardScore, float speed, float shootingDistance, float shootingRate)
{
	enemyID++;
	this->rewardMoney = rewardMoney;
	this->rewardScore = rewardScore;
	this->speed = speed;
	this->shootingDistance = shootingDistance;
	this->shootingRate = shootingRate == 0.f ? 0.000001f : shootingRate;

	objectModel = Model("Assets/Models/spaceship.obj");
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
	TimerManager::CreateTimer(1 / shootingRate, true, "Enemy" + Enemy::enemyID, true, this);
}

void Enemy::getNotified(std::string timerName, bool isCallbackEnabled)
{
	canShoot = true;
}

float currentsTime = 0.0f;
float timersActivation = 2.0f;

void Enemy::generateEnemies(float deltaTime)
{
	// Timer per gestire l'istanza delle monete nel tempo
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
		Game::Instance().InstantiateGameObject(new Enemy(), new Transform(glm::vec3(x, y, -2.0f), eulerAngles, enemyScale));
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