//
// Created by timofey on 25.10.2021.
//
#include "../../../include/segmentation/regionGrowing/regionGrowing.h"
#include "../../../include/io/MeshWriter.h"
#include "../../../include/io/MeshReader.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 3)
        return -1;

    Mesh mesh = MeshReader::readOff(argv[1]);

    Mesh decomposed = regionGrowing::segmentation(mesh);

    MeshWriter::writeOff(argv[2], decomposed);

    return 0;
}