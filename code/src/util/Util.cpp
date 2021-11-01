//
// Created by timofey on 14.10.2021.
//

#include "../../include/util/Util.h"

double util::dot(const Point &v1, const Point &v2)
{
    return v1.x[0] * v2.x[0] + v1.x[1] * v2.x[1] + v1.x[2] * v2.x[2];
}

Point util::cross(const Point& v1, const Point& v2)
{
    Point ans;
    ans.x[0] = v1.x[1] * v2.x[2] - v1.x[2] * v2.x[1];
    ans.x[1] = v1.x[2] * v2.x[0] - v1.x[0] * v2.x[2];
    ans.x[2] = v1.x[0] * v2.x[1] - v1.x[1] * v2.x[0];
    return ans;
}

void util::normalizeV(Point& v)
{
    double len = sqrt(dot(v, v));
    if (len < M_EPS)
        return;
    v.x[0] /= len;
    v.x[1] /= len;
    v.x[2] /= len;
}