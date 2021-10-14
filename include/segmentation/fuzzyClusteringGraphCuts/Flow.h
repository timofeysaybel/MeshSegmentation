//
// Created by timofey on 07.10.2021.
//

#ifndef MESHSEGMENTATION_FLOW_H
#define MESHSEGMENTATION_FLOW_H

#include <iostream>
#include <queue>
#include <vector>

#define FLOW_EPSILON 1e-10
#define INF 1e12

struct Edge
{
    int to, next;
    int from;
    float cap;

    friend std::ostream &operator<<(std::ostream &os, const Edge &e)
    {
        return os << e.from << ", " << e.to << ", " << e.cap;
    }
};

/** Building FlowNet to solve Max Flow Problem */
class FlowNet
{
public:
    int num_v, edges_num;
    std::vector<int> head, last;
    std::vector<Edge> edges;
    std::vector<bool> visit;
    std::vector<float> flow;

    explicit FlowNet(int num_ver);

    void addEdge(int from, int to, float cap);

    bool bfs(int src, int dst);

    float EK(int src, int dst);

    void findCut(int src);
};


#endif //MESHSEGMENTATION_FLOW_H
