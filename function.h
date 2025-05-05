#ifndef _FUNCTION__H
#define _FUNCTION__H

#include <SDL.h>
#include <cmath>
#include <utility>

// góc giữa đường thẳng và Ox
inline std::pair<double, double> getAngle(float x1, float y1, float x2, float y2) {
    double angleRad = atan2(y2 - y1, x2 - x1);
    double angleDeg = angleRad * (180.0 / M_PI);
    return {angleRad, angleDeg};
}

#endif

