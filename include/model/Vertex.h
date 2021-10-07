//
// Created by timofey on 07.10.2021.
//

#ifndef MESHSEGMENTATION_VERTEX_H
#define MESHSEGMENTATION_VERTEX_H


class Vertex
{
public:
    double x[3]{};

    Vertex() = default;

    Vertex(double xx, double y, double z);
};


#endif //MESHSEGMENTATION_VERTEX_H
