#include <iostream>
#include <vector>

#include "include/Reader.h"
#include "include/Writer.h"
#include "include/Util.h"
#include "include/Graph.h"
#include "include/Solver.h"
#include "include/SolverK.h"

using namespace std;
using namespace util;

int main(int argc, char* argv[])
{
    string inputFileName = argv[1];
    string outputFileName = argv[2];

    double eps = 0.04;
    double delta = 0.80;

    if (argc > 3)
        eps = atof(argv[3]);
    if (argc > 4)
        delta = atof(argv[4]);

    Reader reader;
    Model model;
    Graph graph;

    reader.read(inputFileName, model);

    cout << model.faces.size() << " faces" << endl;
    cout << model.vertices.size() << " vertices" << endl;

    model.findNeighFace(delta);
    cout << "distance computed" << endl;

    graph.build(model);
    cout << "graph built" << endl;

    graph.solve();
    cout << "graph solved" << endl;

    vector<int> used;
    for (int i = 0; i < model.faces.size(); ++i)
        used.push_back(i);

    globalSz = model.faces.size();

    Solver* solver = new SolverK(&model, &graph, eps);
    types = 0;

    solver->init(used, 0);
    cout << "init done" << endl;

    solver->process();
    cout << "clustering done" << endl;

    Writer writer;
    writer.write(outputFileName, model);

    cout << types << endl;
    cout << "output done" << endl;

    return 0;
}