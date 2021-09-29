//
// Created by timofey on 28.09.2021.
//
#include <iostream>

#include "../include/Neighbor.h"

Neighbor::Neighbor(int v1, int v2, int f, double da_, double dg_, double a_)
{
    vids[0] = v1;
    vids[1] = v2;
    fid = f;
    da = da_;
    dg = dg_;
    a = a_;
}

Neighbor::Neighbor(const Neighbor& n)
{
    vids[0] = n.vids[0];
    vids[1] = n.vids[1];
    fid = n.fid;
    da = n.da;
    dg = n.dg;
    d = n.d;
    a = n.a;
}

void Neighbor::avg(double avg_da, double avg_dg, double delta)
{
    d = (1 - delta) * da / avg_da + delta * dg / avg_dg;
    if (d < 0)
    {
        std::cout << d << " " << da / avg_da << " " << dg / avg_dg << std::endl;
    }
}
