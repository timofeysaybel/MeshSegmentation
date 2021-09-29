//
// Created by timofey on 28.09.2021.
//
#include <iostream>

#include "../include/Graph.h"

using namespace std;
using namespace util;

void Graph::build(Model& model)
{
    sz = model.faces.size();
    dist.resize(sz);
    weight.resize(sz);
    neigh.resize(sz);
    net.resize(sz + 2);

    for (int i = 0; i < sz; ++i)
    {
        dist[i].resize(sz);
        for(int j = 0; j < sz; ++j)
            dist[i][j] = M_INF;
        dist[i][i] = 0;
    }

    heap.resize(sz + 2);
    p.resize(sz);

    for(int i = 0; i < sz; ++i)
    {
        for (int j = 0; j < model.faces[i].neighs.size(); ++j)
        {
            weight[i].push_back(model.faces[i].neighs[j].d);
            neigh[i].push_back(model.faces[i].neighs[j].fid);
            FlowEdge fe;
            fe.cap = 1.0 / (1 + model.faces[i].neighs[j].da / avg_da);
            fe.flow = 0;
            fe.id = model.faces[i].neighs[j].fid;
            net[i].push_back(fe);
        }
    }

    src = sz;
    dst = sz + 1;
}

void Graph::heapDown(int x)
{
    while (x * 2 <= tail)
    {
        int y = x * 2;
        if (y < tail && dist[s][heap[y]] > dist[s][heap[y + 1]])
            y++;
        if (dist[s][heap[x]] > dist[s][heap[y]])
        {
            int tmp = heap[x];
            heap[x] = heap[y];
            heap[y] = tmp;
            p[heap[x]] = x;
            p[heap[y]] = y;
            x = y;
        }
        else
            break;
    }
}

void Graph::heapUp(int x)
{
    while (x > 1)
    {
        int y = x / 2;
        if (dist[s][heap[x]] < dist[s][heap[y]])
        {
            int tmp = heap[x];
            heap[x] = heap[y];
            heap[y] = tmp;
            p[heap[x]] = x;
            p[heap[y]] = y;
            x = y;
        }
        else
            break;
    }
}

void Graph::solve()
{
    for (s = 0; s < sz; ++s)
    {
        int count = 0;

        for (int i = 0; i < sz; ++i)
            p[i] = 0;

        tail = 1;
        heap[tail] = s;
        p[s] = 1;
        while (tail > 0)
        {
            int now = heap[1];
            heap[1] = heap[tail];
            tail--;
            count++;
            heapDown(1);
            for (int i = 0; i < neigh[now].size(); ++i)
            {
                int next = neigh[now][i];
                double d = weight[now][i];

                if (dist[s][next] > dist[s][now] + d)
                {
                    dist[s][next] = dist[s][now] + d;

                    if (p[next] == 0)
                        p[next] = ++tail;

                    heap[p[next]] = next;
                    heapUp(p[next]);
                    heapDown(p[next]);
                }
            }
        }
    }
}

void Graph::initFlow(vector<int>& m)
{
    mask = m;
    int netSz = 0;
    net[src].clear();
    for (int i = 0; i < sz; ++i)
    {
        if (mask[i] == 1)
        {
            FlowEdge fe;
            fe.cap = M_INF;
            fe.flow = 0.0;
            fe.id = i;
            net[src].push_back(fe);
            netSz++;
        }
        else if (mask[i] == 2)
        {
            int last = net[i].size() - 1;
            if (net[i][last].id != dst)
            {
                FlowEdge fe;
                fe.cap = M_INF;
                fe.flow = 0.0;
                fe.id = dst;
                net[i].push_back(fe);
            }
            netSz++;
        }
        else
        {
            if (mask[i] == 3) netSz++;
            int last = net[i].size() - 1;
            if (net[i][last].id == dst)
                net[i].pop_back();
        }
    }
    cout << "net size: " << netSz << endl;
    mask.push_back(4);
    mask.push_back(4);
    for (int i = 0; i < sz + 2; ++i)
    {
        if (mask[i])
        {
            for (int j = 0; j < net[i].size(); ++j)
                net[i][j].flow = 0.0;
        }
    }
}

void Graph::flow(vector<int>& m)
{
    vector<FlowState> q;
    vector<bool> visit(sz + 2);
    int head = 0, tail = 0;
    int steps = 0;
    double sumFlow = 0;

    while (true)
    {
        steps++;
        head = tail = 0;
        q.clear();

        for (int i = 0; i < visit.size(); i++)
            visit[i] = false;

        FlowState fs;
        fs.id = src;
        fs.front = 0;
        fs.inc = M_INF;
        q.push_back(fs);
        visit[src] = true;

        while (head <= tail && !visit[dst])
        {
            FlowState now = q[head];

            for (int i = 0; i < net[now.id].size(); ++i)
            {
                int next = net[now.id][i].id;
                if (!mask[next]) continue;
                if (visit[next]) continue;
                double inc = net[now.id][i].cap - net[now.id][i].flow;
                if (inc < M_EPS) continue;
                fs.id = next;
                fs.front = head;
                if (now.inc > inc) fs.inc = inc;
                else fs.inc = now.inc;
                tail++;
                q.push_back(fs);
                visit[next] = true;
                if (next == dst) break;
            }
            head++;
        }
        if (!visit[dst])
        {
            for (int i = 1; i <= tail; ++i)
                mask[q[i].id] = 1;

            cout << "tail = " << tail << endl;

            for (int i = 0; i < sz; ++i)
                if (mask[i] == 3) mask[i] = 2;

                break;
        }

        double inc = q[tail].inc;
        sumFlow += inc;

        while (tail != 0)
        {
            int to = q[tail].id;
            int from = q[q[tail].front].id;

            if (inc > 1e8)
                cout << from << "->" << to << endl;

            for (int i = 0; i < net[from].size(); ++i)
                if (net[from][i].id == to)
                    net[from][i].flow += inc;

            for (int i = 0; i < net[to].size(); ++i)
                if (net[to][i].id == from)
                    net[to][i].flow -= inc;

            tail = q[tail].front;
        }
    }
    cout << "steps: " << steps << ", total flow: " << sumFlow << endl;
    m = mask;
}
