//
// Created by timofey on 07.10.2021.
//
#include <iostream>
#include <fstream>

#include "../../include/io/MeshWriter.h"

using namespace std;

void MeshWriter::writeOff(const string& filename, const Mesh& mesh)
{
    ofstream file(filename);

    cout << "Writing in " << filename << endl;

    file << "OFF" << endl;
    file << mesh.vertices.size() << " " << mesh.faces.size() << " 0" << endl;

    for (auto &v : mesh.vertices)
        file << v.p.x[0] << " " << v.p.x[1] << " " << v.p.x[2] << endl;

    for (auto &f : mesh.faces)
    {
        file << "3 " << f.vertices[0] << " " << f.vertices[1] << " " << f.vertices[2] << " ";
        file << 60 * (f.label % 4 + 1) << " " << 80 * ((f.label + 1) % 3 + 1) << " " << 50 * ((f.label + 2) % 5 + 1) << endl;
    }
}
