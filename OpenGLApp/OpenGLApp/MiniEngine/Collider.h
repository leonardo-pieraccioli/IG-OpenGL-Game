#pragma once

#include "Transform.h"

class Collider
{
public:
	Collider(Transform& transform) : transformReference(transform) { };
	~Collider() = default;

private:
	Transform& transformReference;

};

