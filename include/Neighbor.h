//
// Created by timofey on 28.09.2021.
//

#ifndef MESHSEGMENTATION_NEIGHBOR_H
#define MESHSEGMENTATION_NEIGHBOR_H

struct Neighbor
{
    int vids[2];
    int fid;
    double da, dg, d, a;

    Neighbor() {}

    Neighbor(int v1, int v2, int f, double da_, double dg_, double a_);

    Neighbor(const Neighbor& n);

    void avg(double avg_da, double avg_dg, double delta);
};

#endif //MESHSEGMENTATION_NEIGHBOR_H
