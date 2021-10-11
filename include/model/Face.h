//
// Created by timofey on 07.10.2021.
//

#ifndef MESHSEGMENTATION_FACE_H
#define MESHSEGMENTATION_FACE_H

#include <vector>
#include "Point.h"

class Face
{
public:
    int vertices[3]{};
    std::vector<Face> neighbors;

    Point normal;

    int label = 0;

    Face() = default;

    Face(int v1, int v2, int v3);

    Face(const Face& an);

    friend bool operator==(const Face& a, const Face& b);
};


#endif //MESHSEGMENTATION_FACE_H
