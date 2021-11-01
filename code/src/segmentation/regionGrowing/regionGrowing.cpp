//
// Created by timofey on 25.10.2021.
//

#include <vector>
#include <cmath>

#include "../../../include/segmentation/regionGrowing/regionGrowing.h"
#include "../../../include/util/Util.h"

using namespace std;
using namespace util;

/** Finds all neighbors for each face in mesh */
void assignNeighbors(Mesh& mesh);
/** Assigns unique label for each face in mesh */
void initialLabeling(Mesh& mesh);

Mesh regionGrowing::segmentation(const Mesh& mesh)
{
    Mesh decomposed(mesh);

    initialLabeling(decomposed);

    assignNeighbors(decomposed);

    decompose(decomposed);

    return decomposed;
}

void initialLabeling(Mesh& mesh)
{
    for (int i = 0; i < mesh.faces.size(); i++)
    {
        mesh.faces[i].label = i;
        mesh.faces[i].marked = false;
    }
}

/** Returns true if a and b are neighbours */
bool isNeighFace(const Face &a, const Face &b)
{
    std::vector<int> common;
    Indices ia = a.indices, ib = b.indices;
    int same = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (ia[i] == ib[j])
            {
                common.push_back(ia[i]);
                same++;
                break;
            }
        }
    }

    if (same == 2)
        return true;

    return false;
}

void assignNeighbors(Mesh& mesh)
{
    for (int i = 0; i < mesh.faces.size(); i++)
    {
        mesh.faces[i].neighbors.clear();
        for (int j = 0; j < mesh.faces.size(); j++)
        {
            if (isNeighFace(mesh.faces[i], mesh.faces[j]))
                mesh.faces[i].neighbors.push_back(&mesh.faces[j]);
        }
    }
}

double calcDist(const Face& a, const Face& b)
{
    double res = 0.;
    for (int i = 0; i < 3; i++)
        res += (a.center.x[i] - b.center.x[i]) * (a.center.x[i] - b.center.x[i]);
    return sqrt(res);
}

double calculateAvgDist(const Mesh& mesh)
{
    double avg = 0.;
    int count = 0;
    for (int i = 0; i < mesh.faces.size(); i++)
    {
        for (int j = 0; j < mesh.faces[i].neighbors.size(); j++)
        {
            avg += calcDist(mesh.faces[i], *(mesh.faces[i].neighbors[j]));
            count++;
        }
    }
    return avg / count;
}

/** Returns angle between normals of two faces */
double calculateAngle(const Face& a, const Face& b)
{
    return acos(dot(a.normal, b.normal) / (sqrt(dot(a.normal, a.normal)) * sqrt(dot(b.normal, b.normal)))) * 180. / M_PI;
}

void regionGrowing::decompose(Mesh &mesh)
{
    //double avgDist = calculateAvgDist(mesh);
    for (int i = 0; i < mesh.faces.size(); i++)
    {
        for (int j = 0; j < mesh.faces[i].neighbors.size(); j++)
        {
            //double dihedralAngle = M_PI - calculateAngle(mesh.faces[i], *(mesh.faces[i].neighbors[j]));
            if (calculateAngle(mesh.faces[i], *(mesh.faces[i].neighbors[j])) < 360. && !mesh.faces[i].neighbors[j]->marked)
            {
                mesh.faces[i].neighbors[j]->label = mesh.faces[j].label;
                mesh.faces[i].neighbors[j]->marked = true;
            }
        }
    }
}