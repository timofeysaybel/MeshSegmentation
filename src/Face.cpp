//
// Created by timofey on 28.09.2021.
//
#include "../include/Face.h"

using namespace util;

Face::Face()
{
    label = 0;
}

Face::Face(Vertex& v1, Vertex& v2, Vertex& v3)
{
    c = (v1.p + v2.p + v3.p) / 3.0;
    normal = cross(v2.p - v1.p, v3.p - v1.p);
    normalizeV(normal);
    label = 0;
}
Face::Face(const Face& f)
{
    vids[0] = f.vids[0];
    vids[1] = f.vids[1];
    vids[2] = f.vids[2];
    neighs = f.neighs;
    normal = f.normal;
    c = f.c;
    label = f.label;
}