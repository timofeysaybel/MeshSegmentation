//
// Created by timofey on 28.09.2021.
//

#ifndef MESHSEGMENTATION_VERTEX_H
#define MESHSEGMENTATION_VERTEX_H
#include "Point.h"

struct Vertex
{
    Point p;

    Vertex(double x, double y, double z) : p(x, y, z) {}

    Vertex(const Vertex& v);
};

#endif //MESHSEGMENTATION_VERTEX_H
