//
// Created by timofey on 07.10.2021.
//

#ifndef MESHSEGMENTATION_VERTEX_H
#define MESHSEGMENTATION_VERTEX_H


#include "Point.h"

class Vertex
{
public:
    Point p;

    Vertex() = default;

    Vertex(double xx, double y, double z);

    Vertex(const Vertex& an);
};


#endif //MESHSEGMENTATION_VERTEX_H
