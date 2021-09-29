//
// Created by timofey on 28.09.2021.
//

#ifndef MESHSEGMENTATION_GRAPH_H
#define MESHSEGMENTATION_GRAPH_H

#include <vector>

#include "Util.h"
#include "Model.h"

class Graph
{
private:
    std::vector<std::vector<double> > weight;
    std::vector<std::vector<int> > neigh;
    std::vector<int> p;
    std::vector<int> heap;
    int tail;
    int sz;
    int s;

    std::vector<std::vector<util::FlowEdge> > net;
    std::vector<int> mask;
    int src, dst;
public:
    std::vector<std::vector<double> > dist;

    Graph() {}

    void build(Model& model);

    void heapDown(int x);

    void heapUp(int x);

    void solve();

    void initFlow(std::vector<int>& m);

    void flow(std::vector<int>& m);
};

#endif //MESHSEGMENTATION_GRAPH_H
