#include <iostream>

#include "include/model/Mesh.h"
#include "include/io/MeshWriter.h"
#include "include/io/MeshReader.h"

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 3)
        return -1;

    cout << "Reading file " << argv[1] << endl;

    Mesh mesh = MeshReader::readOff(argv[1]);

    cout << "Writing blue model in " << argv[2] << endl;

    MeshWriter::writeOff(argv[2], mesh);

    cout << "Wrote" << endl;

    return 0;
}
