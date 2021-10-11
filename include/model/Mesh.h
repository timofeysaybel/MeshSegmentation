//
// Created by timofey on 07.10.2021.
//

#ifndef MESHSEGMENTATION_MESH_H
#define MESHSEGMENTATION_MESH_H

#include <vector>

#include "Face.h"
#include "Vertex.h"
#include "Edge.h"

class Mesh
{
public:
    std::vector<Face> faces;
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;

    Mesh() = default;

    Mesh(const Mesh& an);
};


#endif //MESHSEGMENTATION_MESH_H
