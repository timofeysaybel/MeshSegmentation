//
// Created by timofey on 28.09.2021.
//

#ifndef MESHSEGMENTATION_EDGE_H
#define MESHSEGMENTATION_EDGE_H

struct Edge
{
    int vids[2];
    int fid;

    Edge() {}

    Edge(int v1, int v2, int f);
};

#endif //MESHSEGMENTATION_EDGE_H
