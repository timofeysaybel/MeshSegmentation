//
// Created by timofey on 28.09.2021.
//
#include <iostream>

#include "../include/Solver2.h"
#include "../include/Util.h"

using namespace std;
using namespace util;

void Solver2::init(vector<int>& u, int l)
{
    level = l;
    used = u;
    num = 2;
    reps.resize(num);
    prob.resize(num);
    sz = used.size();
    for (int i = 0; i < num; ++i)
        prob[i].resize(sz);
    localMaxD = maxDistance();
    if (level == 0)
        globalAvgD = avgDistance();
    localAvgD = avgDistance();
    angRange = angDifference();
}

void Solver2::expand()
{
    double maxD = maxPatchDistance();
    cout << "minPatchD = " << maxD / globalMaxD << endl;
    if (level > 2 || maxD / globalMaxD < TH_REP_DIST_RATIO) return;
    for (int i = 0; i < num; ++i)
    {
        Solver* s = new Solver2(pModel, pGraph, eps);
        vector<int> uNew;
        for (int j = 0; j < sz; ++j)
            if ((pModel->faces[used[j]].label)%num == i)
                uNew.push_back(used[j]);
        s->init(uNew, level + 1);
        sons.push_back(s);
    }
    for (int i = 0; i < num; ++i)
    {
        cout << "maxD = " << sons[i]->localMaxD / globalMaxD
             << " angD = " << sons[i]->angRange << endl;
        if (sons[i]->localMaxD / globalMaxD > TH_DIST_RATIO &&
            sons[i]->angRange > TH_ANGLE_DIFF)
            sons[i]->process();
    }
}
