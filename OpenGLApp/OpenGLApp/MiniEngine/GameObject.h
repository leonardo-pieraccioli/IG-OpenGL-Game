#pragma once
#include <glm/glm.hpp>
#include "Transform.h"
#include "../shader_s.h"
#include "glm/gtc/matrix_transform.hpp"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject() = default;
	virtual void Update(float deltaTime);
	virtual void Draw(Shader ourShader);

	// operators override
	bool operator == (const GameObject& go);
	bool operator != (const GameObject& go);
	
	Transform transform;
	//mesh
	
	//texture
	virtual void SetTextures(unsigned int texture1, unsigned int texture2);
	unsigned int texture1;
	unsigned int texture2;

	unsigned int GetID() const;

private:
	unsigned int ID;
};

