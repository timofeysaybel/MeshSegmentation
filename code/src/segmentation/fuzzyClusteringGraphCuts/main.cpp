//
// Created by timofey on 07.10.2021.
//
#include <iostream>
#include "../../../include/io/MeshReader.h"
#include "../../../include/io/MeshWriter.h"
#include "../../../include/segmentation/fuzzyClusteringGraphCuts/GraphMesh.h"

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 3)
        return -1;

    cout << "Reading file " << argv[1] << endl;

    Mesh mesh = MeshReader::readOff(argv[1]);

    auto* graphMesh = static_cast<GraphMesh*>(&mesh);
    graphMesh->getDual();
    graphMesh->compDist();
    graphMesh->solve();
    graphMesh->writeOff(argv[2]);
    //Mesh* res = graphMesh;
    //MeshWriter::writeOff(argv[2], *res);
}
