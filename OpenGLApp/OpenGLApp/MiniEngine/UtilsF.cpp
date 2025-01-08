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

}
