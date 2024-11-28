#include "GameObject.h"

GameObject::GameObject()
{
	transform = (Transform *) malloc(sizeof(Transform));

}

GameObject::~GameObject()
{
	free(transform);

}

void GameObject::Update(float deltaTime)
{

}


