//
// Created by timofey on 10.10.2021.
//

#ifndef MESHSEGMENTATION_GRAPH_H
#define MESHSEGMENTATION_GRAPH_H

#include <utility>
#include <vector>
#include <map>

#include "../../model/Face.h"
#include "../../model/Mesh.h"

/** Class needed to decompose mesh using graph cut techniques.
 * Сontains mesh with found face neighbors and calculated normals.
 * Weight of the edge is calculated as an angle between faces normals */
class Graph
{
    Mesh mesh;

    /** Calculate noramal for one face */
    Point calculateNormal(const Face& face);

    /** Calculate normal for each face */
    void calculateNormals();

public:
    Graph() = default;

    /** Copy mesh and find neighbors */
    static Graph createFromMesh(const Mesh& mesh);

    /** Get decomposed mesh */
    Mesh getMesh();

    /** Execute mesh segmentation and label faces */
    void decompose(double delta);
};


#endif //MESHSEGMENTATION_GRAPH_H
