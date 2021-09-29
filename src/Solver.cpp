//
// Created by timofey on 28.09.2021.
//
#include "iostream"

#include "../include/Solver.h"

using namespace std;
using namespace util;

Solver::Solver(Model* m, Graph* g, double e)
{
    pModel = m;
    pGraph = g;
    eps = e;
}

void Solver::computeProb()
{
    for (int i = 0; i < sz; ++i)
    {
        bool isRep = false;
        for(int j = 0; j < num; ++j)
        {
            if (used[i] == reps[j])
            {
                isRep = true;

                for (int k = 0; k < num; ++k)
                    prob[k][i] = 0;

                prob[j][i] = 1;
                break;
            }
        }

        if (isRep)
            continue;

        double sum = 0;

        for (int j = 0; j < num; ++j)
            sum += 1.0 / pGraph->dist[used[i]][reps[j]];

        for (int j = 0; j < num; ++j)
            prob[j][i] = 1.0 / pGraph->dist[used[i]][reps[j]] / sum;
    }
}

void Solver::recomputeProb()
{
    vector<int> typeNum(num);
    vector<vector<double> > distNew(num);

    for (int i = 0; i < num; ++i)
    {
        typeNum[i] = 0;
        distNew[i].resize(sz);
        for (int j = 0; j < sz; ++j)
            distNew[i][j] = 0;
    }
    for (int i = 0; i < sz; ++i)
    {
        int tmp = pModel->faces[used[i]].label - types;
        if (tmp < num)
            typeNum[tmp]++;
        else
            continue;

        for (int j = 0; j < sz; ++j)
        {
            distNew[tmp][j] += pGraph->dist[used[i]][used[j]];
        }
    }
    for (int i = 0; i < num; ++i)
    {
        for (int j = 0; j < sz; ++j)
        {
            if (typeNum[i] > 0)
                distNew[i][j] /= typeNum[i];
            else
                distNew[i][j] = M_INF;
        }
    }
    for (int i = 0; i < sz; ++i)
    {
        double sum = 0;

        for (int j = 0; j < num; ++j)
            sum += 1.0 / (distNew[j][i] + M_EPS);

        for (int j = 0; j < num; ++j)
            prob[j][i] = 1.0 / (distNew[j][i] + M_EPS) / sum;
    }
}

bool Solver::updateRep()
{
    bool change = false;
    for (int i = 0; i < num; ++i)
    {
        double minSum = 0.0;
        for (int j = 0; j < sz; ++j)
            minSum += prob[i][j] * pGraph->dist[reps[i]][used[j]];

        for (int j = 0; j < sz; ++j)
        {
            double sum = 0;
            for (int k = 0; k < sz; ++k)
                sum += prob[i][k] * pGraph->dist[used[j]][used[k]];

            if (sum < minSum - M_EPS)
            {
                if (reps[i] != used[j])
                    change = true;

                minSum = sum;
                reps[i] = used[j];
            }
        }
    }
    return change;
}

void Solver::process()
{
    int maxIter = 20;
    mask.resize(num);
    for (int i = 0; i < maxIter; ++i)
    {
        cout << "iter = " << i << endl;
        for (int j = 0; j < num; ++j)
            cout << reps[j] << " ";
        cout << endl;

        computeProb();
        assign();
        recomputeProb();

        if (!updateRep())
            break;
    }
    cout << "converge" << endl;
    assign();
    cout << "after assign 0" << endl;
    recomputeProb();
    cout << "after recompute" << endl;
    assign();
    cout << "after assign 1" << endl;
    cut();
    types += num;
    this->expand();
}
void Solver::assign()
{
    for (int i = 0; i < num; ++i)
    {
        int repeat = 0;
        for (int j = 0; j < i; ++j)
        {
            if (reps[j] == reps[i])
                repeat = 1;
        }
        mask[i] = 1-repeat;
    }

    for (int j = 0; j < num; ++j)
        cout << mask[j] << " ";

    cout << endl;
    int c[20] = {0};
    double diff = eps * globalAvgD / localAvgD;
    if (diff > 0.2)
        diff = 0.2;
    diff = eps;

    cout << "diff = " << diff << endl;

    for (int i = 0; i < sz; i++)
    {
        double maxA = -1.0, maxB = -1.0;
        int labelA = 0, labelB = 0;
        for (int j = 0; j < num; j++)
        {
            if (!mask[j])
                continue;
            if (maxA < prob[j][i])
            {
                maxB = maxA;
                labelB = labelA;
                maxA = prob[j][i];
                labelA = j;
            }
            else if (maxB < prob[j][i])
            {
                maxB = prob[j][i];
                labelB = j;
            }
        }
        if (maxA - maxB > diff)
        {
            pModel->faces[used[i]].label = types+labelA;
            c[labelA]++;
        }
        else
        {
            pModel->faces[used[i]].label = 1024 + (labelA*num) + labelB;
        }
    }
    cout << "split: " << sz << "->";
    for(int j = 0; j < num; ++j)
        cout << c[j] << " ";
    cout << endl;
}
void Solver::cut()
{
    vector<int> mm(globalSz);
    vector<int> mmNew;

    for (int i = 0; i < num; ++i)
    {
        if (!mask[i])
            continue;

        for (int j = i + 1; j < num; ++j)
        {
            if (!mask[j])
                continue;

            for (int k = 0; k < globalSz; ++k)
                mm[k] = 0;

            int nowLabelA = 1024 + i*num + j;
            int nowLabelB = 1024 + j*num + i;
            int fuzzy = 0;

            for(int k = 0; k < sz; ++k)
            {
                if (pModel->faces[used[k]].label == nowLabelA || pModel->faces[used[k]].label == nowLabelB)
                {
                    fuzzy++;
                    mm[used[k]] = 3;
                    for (int x = 0; x < pModel->faces[used[k]].neighs.size(); ++x)
                    {
                        int y = pModel->faces[used[k]].neighs[x].fid;
                        if (pModel->faces[y].label == types + i)
                            mm[y] = 1;
                        else
                            if (pModel->faces[y].label == types + j)
                                mm[y] = 2;
                    }
                }
            }
            cout << "fuzzy: " << fuzzy << endl;
            pGraph->initFlow(mm);
            cout << "after init flow" << endl;
            pGraph->flow(mmNew);
            cout << "after flow" << endl;

            for (int k = 0; k < sz; ++k)
            {
                if (mmNew[used[k]] == 1)
                    pModel->faces[used[k]].label = types + i;
                else
                    if (mmNew[used[k]] == 2)
                        pModel->faces[used[k]].label = types + j;
                    else
                        if (mmNew[used[k]] != 0)
                            cout << "wrong " << mmNew[used[k]] << endl;
            }

            for (int k = 0; k < sz; ++k)
            {
                bool checked = false;
                for (int x = 0; x < pModel->faces[used[k]].neighs.size(); ++x)
                {
                    int y = pModel->faces[used[k]].neighs[x].fid;
                    if (pModel->faces[used[k]].label == pModel->faces[y].label)
                        checked = true;
                }
                if (!checked)
                    cout << "isolated face: " << used[k] << endl;
            }
        }
    }
}
double Solver::avgDistance()
{
    double sum = 0.0;
    for (int i = 0; i < sz; ++i)
        for (int j = i + 1; j < sz; ++j)
            sum += pGraph->dist[used[i]][used[j]];

    sum /= sz*(sz - 1) / 2;
    return sum;
}
double Solver::angDifference()
{
    double maxA = 0.0, minA = M_PI;

    for (int i = 0; i < sz; ++i)
    {
        int iLabel = pModel->faces[used[i]].label;
        for (int j = 0; j < pModel->faces[used[i]].neighs.size(); ++j)
        {
            int k = pModel->faces[used[i]].neighs[j].fid;
            if (k < used[i])
                continue;

            int kLabel = pModel->faces[k].label;
            if (iLabel == kLabel)
            {
                double ang = pModel->faces[used[i]].neighs[j].a;
                if (ang < minA)
                    minA = ang;
                if (ang > maxA)
                    maxA = ang;
            }
        }
    }
    cout << maxA << " " << minA << endl;
    return maxA - minA;
}
double Solver::maxDistance()
{
    double maxD = 0.0;
    for (int i = 0; i < sz; ++i)
    {
        for (int j = i + 1; j < sz; ++j)
        {
            if (maxD < pGraph->dist[used[i]][used[j]])
            {
                if (num == 2)
                {
                    reps[0] = used[i];
                    reps[1] = used[j];
                }
                maxD = pGraph->dist[used[i]][used[j]];
            }
        }
    }

    cout << "in maxDistance " << maxD << endl;
    if (level == 0)
    {
        globalMaxD = maxD;
    }
    return maxD;
}
double Solver::maxPatchDistance()
{
    double maxD = 0;
    for(int i = 0; i < num; ++i)
    {
        for (int j = i + 1; j < num; ++j)
        {
            if (!mask[j])
                continue;
            if (pGraph->dist[reps[i]][reps[j]] > maxD)
                maxD = pGraph->dist[reps[i]][reps[j]];
        }
    }

    cout << "maxD = " << maxD << endl;
    cout << "global " << globalMaxD << endl;
    return maxD;
}
