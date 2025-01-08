#pragma once
#include <utility>

namespace utilsF {
	std::pair<float, float> calculateForwardXY(float alpha, float deltaTime, float oldX, float oldY, float movementRate);
};