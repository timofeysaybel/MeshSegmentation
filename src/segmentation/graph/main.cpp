/** Mesh Segmentation using graph cut techniques
 *
 * Args format: ./meshSegmentation <inputFile>.off <outputFile>.off [delta]
 *
 * Input: <modelName>.off
 * Output: <modelName>.off
 *
 * Algorithm:
 *      1) Read off file and store the result in Mesh class.
 *      2) Build graph (find neighbors for each faceNode).
 *      3) Decompose graph (weight of the edge is a degree between two nodes(faces)).
 *      [3.1) Merge small clusters] - to be done.
 *      4) Get mesh from graph.
 *      5) Write result in off file.
 *
 * Saybel Timofey */

#include <iostream>

#include "../../../include/model/Mesh.h"
#include "../../../include/io/MeshWriter.h"
#include "../../../include/io/MeshReader.h"
#include "../../../include/segmentation/graph/Graph.h"

using namespace std;

// degrees
double delta = 0.;

int main(int argc, char **argv)
{
    if (argc < 3)
        return -1;

    if (argc == 4)
        delta = stod(argv[3]);

    cout << "Reading file " << argv[1] << endl;

    Mesh mesh = MeshReader::readOff(argv[1]);

    cout << "Building graph" << endl;

    Graph graph = Graph::createFromMesh(mesh);

    cout << "Built\nDecomposing graph" << endl;

    graph.decompose(delta);

    MeshWriter::writeOff(argv[2], mesh);

    Mesh newMesh = graph.getMesh();
    MeshWriter::writeOff("../g.off", newMesh);

    cout << "Saved" << endl;

    return 0;
}
