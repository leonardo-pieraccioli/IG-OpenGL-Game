#include "GameObject.h"
#include "Game.h"

static unsigned int lastID = 0;

GameObject::GameObject()
{
	ID = lastID++;
	transform = Transform();
}

bool GameObject::operator==(const GameObject& go)
{
	return go.GetID() == this->GetID();
}

bool GameObject::operator!=(const GameObject& go)
{
	return go.GetID() != this->GetID();
}

void GameObject::Update(float deltaTime)
{
}


unsigned int GameObject::GetID() const { return ID; }


