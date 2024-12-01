#pragma once
#include <glm/glm.hpp>

using namespace glm;

class Transform
{
private:
	


public:
	vec3 position;
	vec3 rotation;
	vec3 scale;

	Transform(vec3 position = vec3(0.f, 0.f, 0.f), vec3 rotation = vec3(0.f, 0.f, 0.f), vec3 scale = vec3(1.f, 1.f, 1.f));

	void setPosition(vec3 position);
	void setPosition(float x = 0.f, float y = 0.f, float z = 0.f);
	void setRotation(vec3 rotation);
	void setRotation(float x = 0.f, float y = 0.f, float z = 0.f);
	void setScale(vec3 scale);
	void setScale(float x = 1.f, float y = 1.f, float z = 1.f);

	vec3 getPosition();
	vec3 getRotation();
	vec3 getScale();


	//~Transform();

};

