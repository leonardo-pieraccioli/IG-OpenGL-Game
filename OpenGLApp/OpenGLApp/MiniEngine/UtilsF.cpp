#include "UtilsF.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace utilsF {

    std::pair<float, float> utilsF::calculateForwardXY(float alpha, float deltaTime, float oldX, float oldY, float movementRate)
    {
        float newX = (cos(alpha * (M_PI/180)) * deltaTime * movementRate) + oldX;
        float newY = (sin(alpha * (M_PI / 180)) * deltaTime * movementRate) + oldY;
        return std::pair<float, float>(newX, newY);
    }

    float distance2DSquare(float a_x, float a_y, float b_x, float b_y)
    {
        std::complex<float> a(a_x, a_y);
        std::complex<float> b(b_x, b_y);
            
        return std::abs(b - a);
    }

    glm::vec3 rotateAroundZ(float deltaAlpha, float oldAlpha, float radius)
    {
        float x = cos((oldAlpha+deltaAlpha) * (M_PI / 180)) * radius;
        float y = sin((oldAlpha + deltaAlpha) * (M_PI / 180)) * radius;
        return glm::vec3(x, y, 0.0f);
    }
    
    float randomNumberInInterval(float min, float max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> distr(min, max);
        return distr(gen);
    }
}