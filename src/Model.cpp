//
// Created by timofey on 28.09.2021.
//
#include <iostream>

#include "../include/Model.h"

using namespace std;
using namespace util;

void Model::findNeighFace(double delta)
{
    double sum_da = 0.0, sum_dg = 0.0;
    int count = 0;

    for(int i = 0; i < edges.size(); ++i)
    {
        for (int j = i + 1; j < edges.size(); ++j)
        {
            if (edges[i].vids[0] == edges[j].vids[1] &&
                edges[i].vids[1] == edges[j].vids[0])
            {
                count++;
                double a = faceAng(faces[edges[i].fid], faces[edges[j].fid]);
                double da = faceAngD(faces[edges[i].fid], faces[edges[j].fid]);
                double dg = faceGeoD(faces[edges[i].fid], faces[edges[j].fid],
                                     vertices[edges[i].vids[0]].p, vertices[edges[i].vids[1]].p);
                sum_da += da;
                sum_dg += dg;
                Neighbor n1(edges[i].vids[0], edges[i].vids[1], edges[j].fid, da, dg, a);
                Neighbor n2(edges[j].vids[0], edges[j].vids[1], edges[i].fid, da, dg, a);
                faces[edges[i].fid].neighs.push_back(n1);
                faces[edges[j].fid].neighs.push_back(n2);
            } else if (edges[i].vids[0] == edges[j].vids[0] &&
                       edges[i].vids[1] == edges[j].vids[1])
            {
                count++;
                double a = faceAng(faces[edges[i].fid], faces[edges[j].fid], true);
                double da = faceAngD(faces[edges[i].fid], faces[edges[j].fid], true);
                double dg = faceGeoD(faces[edges[i].fid], faces[edges[j].fid],
                                     vertices[edges[i].vids[0]].p, vertices[edges[i].vids[1]].p);
                sum_da += da;
                sum_dg += dg;
                Neighbor n1(edges[i].vids[0], edges[i].vids[1], edges[j].fid, da, dg, a);
                Neighbor n2(edges[j].vids[0], edges[j].vids[1], edges[i].fid, da, dg, a);
                faces[edges[i].fid].neighs.push_back(n1);
                faces[edges[j].fid].neighs.push_back(n2);
            }
        }
    }

    avg_da = sum_da / count;
    avg_dg = sum_dg / count;
    cout << "sum_da: " << sum_da << endl;
    cout << "count : " << count << endl;
    cout << "avg_da: " << avg_da << endl;

    for(int i = 0; i < faces.size(); ++i)
    {
        for (int j = 0; j < faces[i].neighs.size(); ++j)
        {
            faces[i].neighs[j].avg(avg_da, avg_dg, delta);
            if (faces[i].neighs[j].d < -M_EPS)
                cout << "negative " << i << " " << j << " " << faces[i].neighs[j].d << endl;
        }
    }
}
