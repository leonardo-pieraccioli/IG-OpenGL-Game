#include "Coin.h"
#include <utility>
#include "Ship.h"   // temp

float random_number(float min, float max);
std::pair<float, float> generateValidCoordinates(float x_min, float x_max, float y_min, float y_max);
static glm::vec3 coinScale = glm::vec3(0.8f, 0.8f, 0.8f);


Coin::Coin(int initialAmount)
{
	moneyAmount = initialAmount;
	// TEMP: colliders are doubled to account for bad mouse click tracking
	colliderCorners[0] = glm::vec3(this->transform.getPosition().x - (this->transform.getScale().x), this->transform.getPosition().y + (this->transform.getScale().y), 0.0f);
	colliderCorners[1] = glm::vec3(this->transform.getPosition().x + (this->transform.getScale().x), this->transform.getPosition().y - (this->transform.getScale().y), 0.0f);
	// -------------------------------------------------------------------
}

Coin::Coin(int initialAmount, Game& SpaceDefender, float x, float y)
{
	moneyAmount = initialAmount;
	// TEMP: colliders are doubled to account for bad mouse click tracking
	colliderCorners[0] = glm::vec3(x - (coinScale.x), y + (coinScale.y), 0.0f);//glm::vec3(5.0f, 5.0f, 0.0f);//glm::vec3(this->transform.getPosition().x - (this->transform.getScale().x / 2), this->transform.getPosition().y + (this->transform.getScale().y / 2), 0.0f);
	colliderCorners[1] = glm::vec3(x + (coinScale.x), y - (coinScale.y), 0.0f);//glm::vec3(this->transform.getPosition().x + (this->transform.getScale().x / 2), this->transform.getPosition().y - (this->transform.getScale().y / 2), 0.0f);
	// -------------------------------------------------------------------
}

void Coin::setMoney(int money)
{
	moneyAmount = money;
}

int Coin::getMoney()
{
	return moneyAmount;
}

bool Coin::shouldDestroy(glm::vec3 mouseWorldCoord)
{
	if (mouseWorldCoord.x >= colliderCorners[0].x && mouseWorldCoord.x <= colliderCorners[1].x && mouseWorldCoord.y <= colliderCorners[0].y && mouseWorldCoord.y >= colliderCorners[1].y) {
		Game::Instance().DestroyGameObject(this);
		return true;
	}
	return false;
}

float currentTime = 1.0f;
float timerActivation = 1.0f;

void Coin::generateCoins(float deltaTime, Game& SpaceDefender)
{
	//timer per gestire istanziazione monete nel tempo
	currentTime += deltaTime;
	if (currentTime >= timerActivation) {
		currentTime = 0;
		std::pair<float, float> coordinates = generateValidCoordinates(MIN_WIDTH, MAX_WIDTH, MIN_HEIGHT, MAX_HEIGHT);
		float x = coordinates.first;
		float y = coordinates.second;
		SpaceDefender.InstantiateGameObject(new Coin(5, SpaceDefender, x, y), new Transform(glm::vec3(x, y, -2.0f), glm::vec3(0.f, 0.f, 0.f), coinScale));
		std::cout << "New coin in position ( " << x << " : " << y << ")" << std::endl;
	}
}

//Genera delle coordinate tali per cui la moneta non compaia sul pianeta
std::pair<float, float> generateValidCoordinates(float x_min, float x_max, float y_min, float y_max) {
	float x, y;
	do {
		x = random_number(x_min, x_max);
		y = random_number(y_min, y_max);
	} while (x >= -3 && x <= 3 && y >= -3 && y <= 3); // Scarta solo se sia x che y sono nell'intervallo proibito
	return std::make_pair(x, y); // Restituisce le coordinate come std::pair
}

//Ritorna un numero random nell'intervallo (min, max)
float random_number(float min, float max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> distr(min, max);
	return distr(gen);
}