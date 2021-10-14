//
// Created by timofey on 07.10.2021.
//

#include "../../include/model/Face.h"
#include "../../include/util/Util.h"

Face::Face(int v1, int v2, int v3)
{
    indices.x = v1;
    indices.y = v2;
    indices.z = v3;
}

bool operator==(const Face &a, const Face &b)
{
    if (a.indices.x == b.indices.x && a.indices.y == b.indices.y && a.indices.z == b.indices.z)
        return true;
    return false;
}

Face::Face(const Face &an)
{
    indices.x = an.indices.x;
    indices.y = an.indices.y;
    indices.z = an.indices.z;

    for (auto& f : an.neighbors)
        neighbors.push_back(f);

    normal = Point(an.normal);

    label = an.label;
}

Face::Face(Indices index, const Point& px, const Point& py, const Point& pz)
{
    indices = index;
    center = (px + py + pz) / 3;
    normal = util::cross(py - px, pz - px);
    util::normalizeV(normal);
}
