#include "Transform.h"

Transform::Transform(vec3 position, vec3 rotation, vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

void Transform::setPosition(vec3 position)
{
	this->position = position;
}

void Transform::setPosition(float x, float y, float z)
{
	position = vec3(x, y, z);
}

void Transform::setRotation(vec3 rotation)
{
	this->rotation = rotation;
}

void Transform::setRotation(float x, float y, float z)
{
	rotation = vec3(x, y, z);
}

void Transform::setScale(vec3 scale)
{
	this->scale = scale;
}

void Transform::setScale(float x, float y, float z)
{
	scale = vec3(x, y, z);
}

vec3 Transform::getPosition()
{
	return position;
}

vec3 Transform::getRotation()
{
	return rotation;
}

vec3 Transform::getScale()
{
	return scale;
}
