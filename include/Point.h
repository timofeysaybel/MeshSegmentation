//
// Created by timofey on 28.09.2021.
//

#ifndef MESHSEGMENTATION_POINT_H
#define MESHSEGMENTATION_POINT_H

struct Point
{
    double x[3];

    Point()
    {}

    Point(double xx, double yy, double zz);

    Point(const Point &p);

    Point operator+(const Point &p) const;

    Point operator-(const Point &p) const;

    Point operator*(double d);

    Point operator/(double d);
};

#endif //MESHSEGMENTATION_POINT_H
