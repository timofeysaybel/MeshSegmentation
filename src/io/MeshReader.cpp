//
// Created by timofey on 07.10.2021.
//
#include <iostream>
#include <fstream>

#include "../../include/io/MeshReader.h"

using namespace std;

vector<string> split(const string& s, const string& delimiter);

Mesh MeshReader::readOff(const std::string& filename)
{
    ifstream file(filename);

    string line;
    getline(file, line);
    while(line == "OFF" || line.empty() || line[0] == '#')
        getline(file, line);

    cout << "Read the title" << endl;
    auto tmp = split(line, " ");
    int vSize = stoi(tmp[0]);
    int fSize = stoi(tmp[1]);

    cout << "Read the first line and got amount of vertices and faces: " << vSize << " " << fSize << endl;
    Mesh result;
    result.vertices.resize(vSize);
    result.faces.resize(fSize);

    for (int i = 0; i < vSize; i++)
    {
        getline(file, line);
        tmp = split(line, " ");
        result.vertices[i] = Vertex(stod(tmp[0]), stod(tmp[1]), stod(tmp[2]));
    }

    cout << "Read all vertices" << endl;

    for (int i = 0; i < fSize; i++)
    {
        getline(file, line);
        tmp = split(line, " ");
        result.faces[i] = Face(stoi(tmp[1]), stoi(tmp[2]), stoi(tmp[3]));
    }

    cout << "Read all faces" << endl;

    return result;
}

vector<string> split(const string& s, const string& delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}