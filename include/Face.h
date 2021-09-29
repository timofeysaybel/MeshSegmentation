//
// Created by timofey on 28.09.2021.
//

#ifndef MESHSEGMENTATION_FACE_H
#define MESHSEGMENTATION_FACE_H

#include <vector>

#include "Util.h"
#include "Neighbor.h"
#include "Vertex.h"

struct Face
{
    int vids[3];
    std::vector<Neighbor> neighs;
    Point normal;
    Point c;
    int label;

    Face();

    Face(Vertex& v1, Vertex& v2, Vertex& v3);

    Face(const Face& f);
};

#endif //MESHSEGMENTATION_FACE_H
