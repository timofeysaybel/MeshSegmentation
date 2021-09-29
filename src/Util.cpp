//
// Created by timofey on 28.09.2021.
//
#include <cmath>

#include "../include/Util.h"

using namespace std;
using namespace util;

double util::dot(const Point& v1, const Point& v2)
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

bool util::isConvex(const Face& fa, const Face& fb)
{
    return dot(fa.normal, fb.c - fa.c) < M_EPS;
}

double util::faceGeoD(const Face& fa, const Face& fb, const Point& p0, const Point& p1)
{
    Point axis = p1 - p0;
    Point va = fa.c - p0;
    Point vb = fb.c - p0;
    double lenAxis = sqrt(dot(axis, axis));
    double lenA = sqrt(dot(va, va));
    double lenB = sqrt(dot(vb, vb));
    double angA = acos(dot(va, axis) / lenA / lenAxis);
    double angB = acos(dot(vb, axis) / lenB / lenAxis);
    double len = lenA*lenA + lenB*lenB - 2 * lenA*lenB*cos(angA + angB);

    return len;
}

double util::faceAngD(const Face& fa, const Face& fb, bool inverse)
{
    double eta = 0.2;

    if (!isConvex(fa, fb))
        eta = 1.0;

    double d = dot(fa.normal, fb.normal);

    return eta * (1 - d);
}

double util::faceAng(const Face& fa, const Face& fb, bool inverse)
{
    double d = dot(fa.normal, fb.normal);
    return acos(d);
}