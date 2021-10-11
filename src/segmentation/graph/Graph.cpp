//
// Created by timofey on 10.10.2021.
//
#include <algorithm>
#include <cmath>
#include <random>
#include <iostream>

#include "../../../include/segmentation/graph/Graph.h"
#include "../../../include/util/Util.h"

#define MAX_LABEL 10

using namespace std;

/** Find all neighbors for face */
Face findNeighbors(const Face& face, const Mesh& mesh)
{
    Face res = Face(face);

    for (int i = 0; i < mesh.faces.size(); i++)
    {
        if (mesh.faces[i] == face)
            continue;

        if (face.vertices[0] == mesh.faces[i].vertices[0] ||\
            face.vertices[0] == mesh.faces[i].vertices[1] ||\
            face.vertices[0] == mesh.faces[i].vertices[2])
        {
            res.neighbors.emplace_back(mesh.faces[i]);
            continue;
        }

        if (face.vertices[1] == mesh.faces[i].vertices[0] ||\
            face.vertices[1] == mesh.faces[i].vertices[1] ||\
            face.vertices[1] == mesh.faces[i].vertices[2])
        {
            res.neighbors.emplace_back(mesh.faces[i]);
            continue;
        }

        if (face.vertices[2] == mesh.faces[i].vertices[0] ||\
            face.vertices[2] == mesh.faces[i].vertices[1] ||\
            face.vertices[2] == mesh.faces[i].vertices[2])
        {
            res.neighbors.emplace_back(mesh.faces[i]);
            continue;
        }
    }

    return res;
}

/** Calculate angle between 2 vectors
 * returns angle in degrees */
double calcAngle(const Point& v1, const Point& v2)
{
    return acos(util::dot(v1, v2) / (sqrt(util::dot(v1, v1)) * sqrt(util::dot(v2, v2)))) * 180. / M_PI;
}

Graph Graph::createFromMesh(const Mesh& mesh)
{
    Graph graph;

    graph.mesh = Mesh(mesh);

    graph.calculateNormals();

    cout << "\tCalculated normals" << endl;

    for (auto & face : graph.mesh.faces)
        face = findNeighbors(face, mesh);

    cout << "\tFound neighbors" << endl;

    return graph;
}

Mesh Graph::getMesh()
{
    return mesh;
}

void Graph::decompose(double delta)
{
    srand(time(NULL));

    for (auto& f : mesh.faces)
    {
        for (auto& n : f.neighbors)
            n.normal = calculateNormal(n);
    }

    //Initial labeling
    for (int i = 0; i < mesh.faces.size(); i++)
        mesh.faces[i].label = rand() % (MAX_LABEL + 1);

    for (int i = 0; i < mesh.faces.size(); i++)
    {
        for (int j = 0; j < mesh.faces[i].neighbors.size(); j++)
        {
            if (calcAngle(mesh.faces[i].normal, mesh.faces[i].neighbors[j].normal) <= delta)
                mesh.faces[i].neighbors[j].label = mesh.faces[i].label;
        }
    }

    for (int i = 0; i < mesh.faces.size(); i++)
    {
        for (int j = 0; j < mesh.faces[i].neighbors.size(); j++)
        {
            auto pos = distance(mesh.faces.begin(),
                                find(mesh.faces.begin(), mesh.faces.end(),
                                     mesh.faces[i].neighbors[j]));
            mesh.faces[pos].label = mesh.faces[i].neighbors[j].label;
        }
    }
}

void Graph::calculateNormals()
{
    for (auto &f : mesh.faces)
        f.normal = calculateNormal(f);
}

Point Graph::calculateNormal(const Face &face)
{
    Point normal;

    Point v1(mesh.vertices[face.vertices[0]].p);
    Point v2(mesh.vertices[face.vertices[1]].p);
    Point v3(mesh.vertices[face.vertices[2]].p);

    normal = util::cross(v2 - v1, v3 - v1);
    util::normalizeV(normal);

    return normal;
}
