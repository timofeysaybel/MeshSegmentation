//
// Created by timofey on 07.10.2021.
//

#include "../../include/model/Mesh.h"

Mesh::Mesh(const Mesh &an)
{
    faces = an.faces;
    vertices = an.vertices;
    edges = an.edges;
}
