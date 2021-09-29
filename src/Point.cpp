//
// Created by timofey on 28.09.2021.
//
#include "../include/Point.h"

Point::Point(double xx, double yy, double zz)
{
    x[0] = xx;
    x[1] = yy;
    x[2] = zz;
}

Point::Point(const Point& p)
{
    x[0] = p.x[0];
    x[1] = p.x[1];
    x[2] = p.x[2];
}

Point Point::operator+(const Point &p) const
{
    Point ans;

    for (int i = 0; i < 3; ++i)
        ans.x[i] = x[i] + p.x[i];

    return ans;
}

Point Point::operator-(const Point &p) const
{
    Point ans;

    for (int i = 0; i < 3; ++i)
        ans.x[i] = x[i] - p.x[i];

    return ans;
}

Point Point::operator*(double d)
{
    Point ans;

    for (int i = 0; i < 3; ++i)
        ans.x[i] = x[i] * d;

    return ans;
}

Point Point::operator/(double d)
{
    Point ans;

    for (int i = 0; i < 3; ++i)
        ans.x[i] = x[i] / d;

    return ans;
}