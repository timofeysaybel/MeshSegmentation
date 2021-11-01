//
// Created by timofey on 10.10.2021.
//

#include "../../include/model/Point.h"

Point::Point(double xx, double yy, double zz)
{
    x[0] = xx; x[1] = yy; x[2] = zz;
}

Point Point::operator+(const Point &p) const
{
    Point res;
    for (int i = 0; i < 3; ++i)
        res.x[i] = x[i] + p.x[i];
    return res;
}

Point Point::operator-(const Point &p) const
{
    Point res;
    for (int i = 0; i < 3; ++i)
        res.x[i] = x[i] - p.x[i];
    return res;
}

Point Point::operator*(double d)
{
    Point res;
    for (int i = 0; i < 3; ++i)
        res.x[i] = x[i] * d;
    return res;
}

Point Point::operator/(double d)
{
    Point res;
    for (int i = 0; i < 3; ++i)
        res.x[i] = x[i] / d;
    return res;
}

Point::Point(const Point &p)
{
    x[0] = p.x[0]; x[1] = p.x[1]; x[2] = p.x[2];
}

std::ostream &operator<<(std::ostream &stream, const Point &p)
{
    stream << "(" << p.x[0] << "," << p.x[1] << "," << p.x[2] << ")";

    return stream;
}
