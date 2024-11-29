#pragma once
#include <glm/glm.hpp>
#include "Transform.h"

class GameObject
{
public:
	GameObject();
	//~GameObject();
	virtual void Update(float deltaTime);

	Transform transform;
	//mesh
	//texture

private:

};

