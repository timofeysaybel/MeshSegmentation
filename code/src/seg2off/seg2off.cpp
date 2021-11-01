//
// Created by timofey on 25.10.2021.
//
#include <fstream>

#include "../../include/model/Mesh.h"
#include "../../include/io/MeshReader.h"
#include "../../include/io/MeshWriter.h"
#include "../../include/seg2off/seg2off.h"

using namespace std;

void seg2off(string offFile, string segFile, string outFile)
{
    Mesh mesh = MeshReader::readOff(offFile);
    ifstream seg(segFile);
    string str;

    int ind = 0;
    while (getline(seg, str))
    {
        int lbl = stoi(str);
        mesh.faces[ind].label = lbl;
        ind++;
    }

    MeshWriter::writeOff(outFile, mesh);
}