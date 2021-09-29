//
// Created by timofey on 28.09.2021.
//

#ifndef MESHSEGMENTATION_SOLVER_H
#define MESHSEGMENTATION_SOLVER_H

#include <iostream>
#include "Model.h"
#include "Graph.h"

class Solver
{
public:
    Model* pModel;
    Graph* pGraph;
    double eps;
    std::vector<int> used;
    std::vector<int> reps;
    std::vector<std::vector<double> > prob;
    std::vector<Solver*> sons;
    std::vector<bool> mask;
    int num;
    int sz;
    int level;
    double localAvgD;
    double localMaxD;
    double angRange;

    virtual void expand()
    { std::cout << "!!!" << std::endl; }

    virtual void init(std::vector<int>& u, int l)
    { std::cout << "???" << std::endl; }

    Solver() {}

    Solver(Model* m, Graph* g, double e = 0.0);

    void computeProb();

    void recomputeProb();

    bool updateRep();

    void process();

    void assign();

    void cut();

    double avgDistance();

    double angDifference();

    double maxDistance();

    double maxPatchDistance();
};


#endif //MESHSEGMENTATION_SOLVER_H
