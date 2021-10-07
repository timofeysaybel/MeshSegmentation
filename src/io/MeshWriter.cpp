//
// Created by timofey on 07.10.2021.
//

#include <fstream>

#include "../../include/io/MeshWriter.h"

using namespace std;

void MeshWriter::writeOff(const string& filename, const Mesh& mesh)
{
    ofstream file(filename);

    file << "OFF" << endl;
    file << mesh.vertices.size() << " " << mesh.faces.size() << " 0" << endl;

    for (auto &v : mesh.vertices)
        file << v.x[0] << " " << v.x[1] << " " << v.x[2] << endl;

    for (auto &f : mesh.faces)
        file << "3 " << f.vertices[0] << " " << f.vertices[1] << " " << f.vertices[2] << " 0 0 255" << endl;
}
