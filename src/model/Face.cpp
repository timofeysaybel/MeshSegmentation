//
// Created by timofey on 07.10.2021.
//

#include "../../include/model/Face.h"

Face::Face(int v1, int v2, int v3)
{
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;
}

bool operator==(const Face &a, const Face &b)
{
    if (a.vertices[0] == b.vertices[0] && a.vertices[1] == b.vertices[1] && a.vertices[2] == b.vertices[2])
        return true;
    return false;
}

Face::Face(const Face &an)
{
    vertices[0] = an.vertices[0];
    vertices[1] = an.vertices[1];
    vertices[2] = an.vertices[2];

    for (auto& f : an.neighbors)
        neighbors.push_back(f);

    normal = Point(an.normal);

    label = an.label;
}
