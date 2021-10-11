//
// Created by timofey on 10.10.2021.
//

#ifndef MESHSEGMENTATION_EDGE_H
#define MESHSEGMENTATION_EDGE_H


class Edge
{
public:
    int vertices[2];

    Edge() = default;

    Edge(int v1, int v2);
};


#endif //MESHSEGMENTATION_EDGE_H
