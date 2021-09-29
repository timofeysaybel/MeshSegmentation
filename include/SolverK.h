//
// Created by timofey on 28.09.2021.
//

#ifndef MESHSEGMENTATION_SOLVERK_H
#define MESHSEGMENTATION_SOLVERK_H

#include "Solver.h"

class SolverK : public Solver
{
public:
    SolverK() {}

    SolverK(Model* m, Graph* g, double e = 0.0) : Solver(m, g, e) {}

    void init(std::vector<int>& u, int l);

    void expand();
};

#endif //MESHSEGMENTATION_SOLVERK_H
