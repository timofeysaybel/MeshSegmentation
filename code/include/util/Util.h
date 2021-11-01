//
// Created by timofey on 10.10.2021.
//

#ifndef MESHSEGMENTATION_UTIL_H
#define MESHSEGMENTATION_UTIL_H

#include <cmath>

#include "../model/Point.h"

#define M_EPS 1e-12

namespace util
{
    double dot(const Point& v1, const Point& v2);

    Point cross(const Point& v1, const Point& v2);

    void normalizeV(Point& v);
}

#endif //MESHSEGMENTATION_UTIL_H
