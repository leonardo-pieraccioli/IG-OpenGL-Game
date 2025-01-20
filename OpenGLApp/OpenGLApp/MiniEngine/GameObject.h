#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "Transform.h"
#include "../Shader.h"
#include "assimp_API/model.h"

class GameObject
{
public:
	GameObject();

	virtual ~GameObject() = default;
	virtual void Update(float deltaTime);
	virtual void Draw(Shader shader);

	// operators override
	bool operator == (const GameObject& go);
	bool operator != (const GameObject& go);
	
	bool isActive = false;
	Transform transform;
	Model objectModel;

	unsigned int GetID() const;

private:
	unsigned int ID;
};

