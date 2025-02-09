#pragma once
#include "MiniEngine/GameObject.h"
#include "MiniEngine/Game.h"
#include "IObserver.h"
#include "TimerManager.h"

const static glm::vec3 collectableScale = glm::vec3(0.25f, 0.25f, 0.25f);

class Collectables : public GameObject, IObserver
{
protected:
	glm::vec3 colliderCorners[2];
	float despawnTime;
	Timer* despawnTimer;

public:
	Collectables(float despawnTime, unsigned int texture);
	Collectables(float despawnTime, float x, float y);

	void initDestroyTimer();

	void Update(float deltaTime) override;

	bool doesCollectableOverlap(glm::vec3 mouseWorldCoord);

	void getNotified(std::string timerName, bool isCallbackEnabled) override;
};

