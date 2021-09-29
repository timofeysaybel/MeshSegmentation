//
// Created by timofey on 28.09.2021.
//

#ifndef MESHSEGMENTATION_SOLVER2_H
#define MESHSEGMENTATION_SOLVER2_H

#include "Solver.h"

class Solver2 : public Solver
{
public:
    Solver2() {}
    Solver2(Model* m, Graph* g, double e = 0.0) : Solver(m, g, e) {}

    void init(std::vector<int>& u, int l);

    void expand();
};

#endif //MESHSEGMENTATION_SOLVER2_H
