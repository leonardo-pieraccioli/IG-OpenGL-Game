#pragma once
#include <glm/glm.hpp>
#include "Transform.h"
#include "../shader_s.h"

class GameObject
{
public:
	GameObject();
	//~GameObject();
	virtual void Update(float deltaTime);
	void Draw(Shader ourShader);

	// operators override
	bool operator == (const GameObject& go);
	bool operator != (const GameObject& go);
	
	Transform transform;
	//mesh
	//texture

	unsigned int GetID() const;

private:
	unsigned int ID;
};

