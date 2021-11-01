//
// Created by timofey on 07.10.2021.
//

#include "../../include/model/Vertex.h"

Vertex::Vertex(double xx, double y, double z)
{
    p.x[0] = xx;
    p.x[1] = y;
    p.x[2] = z;
}

Vertex::Vertex(const Vertex &an)
{
    p.x[0] = an.p.x[0];
    p.x[1] = an.p.x[1];
    p.x[2] = an.p.x[2];
}
