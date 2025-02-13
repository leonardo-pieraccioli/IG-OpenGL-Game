#include "Planet.h"
#include "MiniEngine/Game.h"

void Planet::rotatePlanet(float deltaTime)
{
	this->transform.rotation.y += rotationRate * deltaTime;
}

void Planet::explode()
{
	Game::Instance().ChangeGameState(GameState::GameOver);
	//Explosion, Game Over
}

Planet::Planet(float rotationRate)
{
	this->rotationRate = rotationRate;
	objectModel = Model("Assets/Models/Planet.obj");
	this->tag = "Planet";
}

void Planet::Update(float deltaTime)
{
	rotatePlanet(deltaTime);
}

void Planet::Damage(int damage)
{
	if (health.Damage(damage) <= 0) {
		explode();
	}
}

void Planet::resetPlanet()
{
	resetUpgrades();
	health.resetHealth();
}

void Planet::upgrade(UpgradeIndex upgradeIndex)
{
	int intUpgradeIndex = static_cast<int>(upgradeIndex);

	switch (intUpgradeIndex) {
		case 5:
			health.UpgradeMax(UpgradeManager::Instance().getGenericCurrentValue(upgradeIndex));
			break;
		default:
			cout << "Errore, upgradeIndex fuori dal range accettabile dalla classe Planet" << endl;
	}
}

void Planet::resetUpgrades()
{
	health.UpgradeMax(INITIAL_HEALTH);
}

