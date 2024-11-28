#pragma once
#include <glm/glm.hpp>

typedef struct {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
} Transform;

class GameObject
{
public:
	GameObject();
	~GameObject();
	virtual void Update(float deltaTime);

	Transform *transform;

private:

};

