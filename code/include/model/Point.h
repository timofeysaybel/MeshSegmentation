//
// Created by timofey on 10.10.2021.
//

#ifndef MESHSEGMENTATION_POINT_H
#define MESHSEGMENTATION_POINT_H

#include <iostream>

class Point
{
public:
    double x[3]{};

    Point(double xx, double yy, double zz);

    Point(const Point &p);

    Point operator+(const Point &p) const;

    Point operator-(const Point &p) const;

    Point operator*(double d);

    Point operator/(double d);

    Point() = default;

    friend std::ostream& operator<<(std::ostream& stream, const Point& p);
};


#endif //MESHSEGMENTATION_POINT_H
