//
// Created by timofey on 28.09.2021.
//

#ifndef MESHSEGMENTATION_MODEL_H
#define MESHSEGMENTATION_MODEL_H

#include "Vertex.h"
#include "Face.h"
#include "Edge.h"

struct Model
{
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    std::vector<Edge> edges;

    void findNeighFace(double delta);
};

#endif //MESHSEGMENTATION_MODEL_H
