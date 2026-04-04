#pragma once
#include <utility>
#include <complex>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace utilsF {
	std::pair<float, float> calculateForwardXY(float alpha, float deltaTime, float oldX, float oldY, float movementRate);

	float distance2DSquare(float a_x, float a_y, float b_x, float b_y);

	glm::vec3 rotateAroundZ(float deltaAlpha, float oldAlpha, float radius);

	float randomNumberInInterval(float min, float max);

	float lerp(float a, float b, float t);

	float interpolateOnRadiuses(float a_x, float a_y, float ship_z, float minR, float maxR);

	std::pair<float, float> generateVibrationCoords(float radius);
};