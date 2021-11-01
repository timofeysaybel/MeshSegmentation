//
// Created by timofey on 07.10.2021.
//

#ifndef MESHSEGMENTATION_INDICES_H
#define MESHSEGMENTATION_INDICES_H


#include <ostream>
#include <cassert>

/** Contains integer indices of vertices for each face */
class Indices
{
public:
    int x, y, z;

    Indices() : x(0), y(0), z(0)
    {}

    explicit Indices(int xx) : x(xx), y(xx), z(xx)
    {}

    Indices(int xx, int yy, int zz) : x(xx), y(yy), z(zz)
    {}

    Indices operator*(const int &r) const
    {
        return {x * r, y * r, z * r};
    }

    Indices operator*(const Indices &v) const
    {
        return {x * v.x, y * v.y, z * v.z};
    }

    Indices operator-(const Indices &v) const
    {
        return {x - v.x, y - v.y, z - v.z};
    }

    Indices operator+(const Indices &v) const
    {
        return {x + v.x, y + v.y, z + v.z};
    }

    int operator[](int i) const
    {
        if (i == 0)
        {
            return x;
        } else if (i == 1)
        {
            return y;
        }
        return z;
    }

    friend std::ostream &operator<<(std::ostream &os, const Indices &v)
    {
        return os << v.x << ", " << v.y << ", " << v.z;
    }
};


#endif //MESHSEGMENTATION_INDICES_H
