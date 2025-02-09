#include "Collectables.h"

Collectables::Collectables(float despawnTime, unsigned int texture)
{
	this->despawnTime = despawnTime;
	// TEMP: colliders are doubled to account for bad mouse click tracking
	colliderCorners[0] = glm::vec3(this->transform.getPosition().x - (this->transform.getScale().x), this->transform.getPosition().y + (this->transform.getScale().y), 0.0f);
	colliderCorners[1] = glm::vec3(this->transform.getPosition().x + (this->transform.getScale().x), this->transform.getPosition().y - (this->transform.getScale().y), 0.0f);
	// -------------------------------------------------------------------

	//objectModel = Model("Assets/Models/Coin.obj");
}

Collectables::Collectables(float despawnTime, float x, float y)
{
	this->despawnTime = despawnTime;
	// TEMP: colliders are doubled to account for bad mouse click tracking
	colliderCorners[0] = glm::vec3(x - (collectableScale.x), y + (collectableScale.y), 0.0f);//glm::vec3(5.0f, 5.0f, 0.0f);//glm::vec3(this->transform.getPosition().x - (this->transform.getScale().x / 2), this->transform.getPosition().y + (this->transform.getScale().y / 2), 0.0f);
	colliderCorners[1] = glm::vec3(x + (collectableScale.x), y - (collectableScale.y), 0.0f);//glm::vec3(this->transform.getPosition().x + (this->transform.getScale().x / 2), this->transform.getPosition().y - (this->transform.getScale().y / 2), 0.0f);
	// -------------------------------------------------------------------

	//objectModel = Model("Assets/Models/Coin.obj");
}

void Collectables::initDestroyTimer()
{
	despawnTimer = TimerManager::CreateTimer(despawnTime, true, "Collectable" + std::to_string(this->GetID()), true, this);
}

void Collectables::Update(float deltaTime)
{
	transform.rotation.z += deltaTime * 50.f;
}

bool Collectables::doesCollectableOverlap(glm::vec3 mouseWorldCoord)
{
	return (mouseWorldCoord.x >= colliderCorners[0].x && mouseWorldCoord.x <= colliderCorners[1].x && mouseWorldCoord.y <= colliderCorners[0].y && mouseWorldCoord.y >= colliderCorners[1].y) ? true : false;;
}

void Collectables::getNotified(std::string timerName, bool isCallbackEnabled)
{
	Game::Instance().DestroyGameObject(this);
}
