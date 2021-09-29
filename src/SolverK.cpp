//
// Created by timofey on 28.09.2021.
//
#include <iostream>

#include "../include/SolverK.h"

using namespace std;
using namespace util;

void SolverK::init(vector<int>& u, int l)
{
    level = l;
    used = u;
    sz = used.size();
    double minSumD = M_INF;
    int choice = 0;

    for (int i = 0; i < sz; ++i)
    {
        double sumD = 0;
        for (int j = 0; j < sz; ++j)
            sumD += pGraph->dist[used[i]][used[j]];
        if (sumD < minSumD)
        {
            minSumD = sumD;
            choice = used[i];
        }
    }

    reps.push_back(choice);
    vector<double> G;
    double maxD = 0;

    for (int i = 1; i < 20; ++i)
    {
        maxD = 0;
        choice = 0;
        for (int j = 0; j < sz; ++j)
        {
            double minD = M_INF;
            for (int k = 0; k < i; ++k)
                if (minD > pGraph->dist[reps[k]][used[j]])
                    minD = pGraph->dist[reps[k]][used[j]];
            if (minD > maxD)
            {
                maxD = minD;
                choice = used[j];
            }
        }
        reps.push_back(choice);
        G.push_back(maxD);
    }

    maxD = G[0] - G[1];
    num = 2;

    for (int i = 0; i < G.size(); ++i)
        cout << G[i] << " ";
    cout << endl;

    for(int i = 1; i < 18; ++i)
    {
        if (maxD < G[i] - G[i + 1])
        {
            maxD = G[i] - G[i + 1];
            num = i + 2;
        }
    }
    cout << "num = " << num << endl;

    while (reps.size() > num)
        reps.pop_back();

    prob.resize(num);

    for (int i = 0; i < num; ++i)
        prob[i].resize(sz);

    localMaxD = maxDistance();

    if (level == 0)
        globalAvgD = avgDistance();

    localAvgD = avgDistance();
    angRange = angDifference();
}

void SolverK::expand()
{
    double maxD = maxPatchDistance();
    cout << "minPatchD = " << maxD / globalMaxD << endl;

    if (level > 0 || maxD / globalMaxD < TH_REP_DIST_RATIO)
        return;

    for (int i = 0; i < num; ++i)
    {
        Solver* s = nullptr;
        if (mask[i])
        {
            s = new SolverK(pModel, pGraph, eps);
            vector<int> uNew;
            for (int j = 0; j < sz; ++j)
                if ((pModel->faces[used[j]].label) % num == i)
                    uNew.push_back(used[j]);

            s->init(uNew, level + 1);
        }
        sons.push_back(s);
    }

    for (int i = 0; i < num; ++i)
    {
        if (!mask[i])
            continue;

        cout << "avgD = " << sons[i]->localAvgD / globalAvgD
             << " angD = " << sons[i]->angRange << endl;
        if (sons[i]->localAvgD / globalAvgD > TH_DIST_RATIO &&
            sons[i]->angRange > TH_ANGLE_DIFF)
            sons[i]->process();
    }
}
