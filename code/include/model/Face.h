//
// Created by timofey on 07.10.2021.
//

#ifndef MESHSEGMENTATION_FACE_H
#define MESHSEGMENTATION_FACE_H

#include <vector>
#include "Point.h"
#include "Indices.h"

/** Structure to store neighbor`s faces*/
struct DualEdge
{
    int face;
    float weight;
    float angle;
    float angle_dist, geo_dist;

    DualEdge(int f, float a_dist, float g_dist, float ang)
            : face(f),
              angle_dist(a_dist),
              geo_dist(g_dist),
              angle(ang),
              weight(0.0f)
    {}

    friend std::ostream &operator<<(std::ostream &os, const DualEdge &de)
    {
        return os << de.face << ", " << de.weight << ", " << de.angle_dist << ", "
                  << de.geo_dist;
    }
};

class Face
{
public:

    Indices indices;
    std::vector<Face*> neighbors;
    std::vector<DualEdge> dedges;  // Neighbor faces for Graph Cut

    Point normal;
    Point center;

    int label = 0;
    bool marked = false;

    Face() = default;

    Face(int v1, int v2, int v3);

    Face(Indices index, const Point& px, const Point& py, const Point& pz);

    Face(const Face &an);

    friend bool operator==(const Face &a, const Face &b);

    friend bool operator<(const Face &a, const Face &b);
};


#endif //MESHSEGMENTATION_FACE_H
