//
// Created by timofey on 07.10.2021.
//

#include <fstream>
#include <cmath>
#include <queue>
#include <set>
#include "../../../include/segmentation/fuzzyClusteringGraphCuts/GraphMesh.h"
#include "../../../include/segmentation/fuzzyClusteringGraphCuts/Flow.h"
#include "../../../include/util/Util.h"

/** Find |v| */
inline float vlen(const Point &v)
{
    float mag = v.x[0] * v.x[0] + v.x[1] * v.x[1] + v.x[2] * v.x[2];
    return sqrtf(mag);
}

bool GraphMesh::isNeighFace(const Face &a, const Face &b, std::vector<int> &common)
{
    common.clear();
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

    if (same == 3)
    {
        std::cout << "Same face !!!" << std::endl;
    }
    return false;
}

double GraphMesh::angleDist(const Face &a, const Face &b, float &angle)
{
    float dot = util::dot(a.normal, b.normal);
    if (dot >= 1.0f)
        dot = 1.0f;
    else if (dot <= -1.0f)
        dot = -1.0f;
    angle = asin(dot);
    float angle_dist = 1 - cos(angle);
    if (util::dot(b.normal - a.normal, a.center - b.center) < 1e-12)
    {
        angle_dist *= 0.2;
    }
    return angle_dist;
}

double GraphMesh::geoDist(const Face &a, const Face &b, const std::vector<int> &common)
{
    assert(common.size() == 2);
    Point p0 = vertices[common[0]].p, p1 = vertices[common[1]].p;
    Point ce = p1 - p0, va = a.center - p0, vb = b.center - p0;
    float l = vlen(ce), la = vlen(va), lb = vlen(vb);
    float angle = acos(util::dot(va, ce) / (la * l)) + acos(util::dot(vb, ce) / (lb * l));
    return la * la + lb * lb - 2 * la * lb * cos(angle);
}

void GraphMesh::getDual()
{
    int num_neigh = 0;
    float tot_angle_dist = 0.0f, tot_geo_dist = 0.0f;
    for (int i = 0; i < num_faces; i++)
    {
        for (int j = i + 1; j < num_faces; j++)
        {
            std::vector<int> common;
            if (isNeighFace(faces[i], faces[j], common))
            {
                num_neigh++;
                float angle;
                float angle_dist = angleDist(faces[i], faces[j], angle);
                float geo_dist = geoDist(faces[i], faces[j], common);
                tot_angle_dist += angle_dist;
                tot_geo_dist += geo_dist;

                faces[i].dedges.emplace_back(j, angle_dist, geo_dist, angle);
                faces[j].dedges.emplace_back(i, angle_dist, geo_dist, angle);
            }
        }
    }
    avg_angle_dist = tot_angle_dist / num_neigh;
    float avg_geo_dist = tot_geo_dist / num_neigh;
    for (Face &f: faces)
    {
        for (DualEdge &de: f.dedges)
        {
            de.weight = 0.2 * de.angle_dist / avg_angle_dist +
                        0.8 * de.geo_dist / avg_geo_dist;
        }
    }
    std::cout << "Num neighbor faces " << num_neigh << std::endl;
    std::cout << "Avg Angle dist " << avg_angle_dist << std::endl;
    std::cout << "Avg Geo dist " << avg_geo_dist << std::endl;
}

void GraphMesh::compDist()
{
    for (int i = 0; i < num_faces; i++)
    {
        std::vector<float> tmp_dist(num_faces, INF_FLOAT);
        dijkstra(i, tmp_dist);
        distance.push_back(tmp_dist);
    }
    std::cout << "Finish computing shortest path" << std::endl;
}

void GraphMesh::dijkstra(int src, std::vector<float> &tmp_dist)
{
    std::priority_queue<fipair, std::vector<fipair>, std::greater<>> pq;
    pq.push(std::make_pair(0.0f, src));
    tmp_dist[src] = 0;
    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();
        for (DualEdge &de: faces[u].dedges)
        {
            int v = de.face;
            float weight = de.weight;
            if (tmp_dist[v] > tmp_dist[u] + weight)
            {
                tmp_dist[v] = tmp_dist[u] + weight;
                pq.push(std::make_pair(tmp_dist[v], v));
            }
        }
    }
}

//vs - vertices used in a decomposition
void GraphMesh::fuzzyCluster(std::vector<int> &vs, std::map<int, int> &part, int &fuzzy, int repA, int repB)
{
    int nv = vs.size();
    prob.reserve(nv);
    std::cout << "Representatives " << repA << " " << repB << std::endl;
    for (int iter = 0; iter < 10; iter++)
    {
        //Compute probabilities
        for (int t = 0; t < nv; t++)
        {
            int v = vs[t];
            float a_fi = distance[v][repA], b_fi = distance[v][repB];
            prob[t] = a_fi / (a_fi + b_fi);
        }

        //Find new representatives
        int lastA = repA, lastB = repB;
        float min_repB = INF_FLOAT, min_repA = INF_FLOAT;
        for (int i = 0; i < nv; i++)
        {
            int v1 = vs[i];
            int tmp_repB = 0.0f, tmp_repA = 0.0f;
            for (int j = 0; j < nv; j++)
            {
                int v2 = vs[j];
                tmp_repB += prob[j] * distance[v1][v2];
                tmp_repA += (1.0 - prob[j]) * distance[v1][v2];
            }
            if (tmp_repB < min_repB)
            {
                min_repB = tmp_repB;
                repB = v1;
            }
            if (tmp_repA < min_repA)
            {
                min_repA = tmp_repA;
                repA = v1;
            }
        }
        std::cout << repA << " " << repB << std::endl;
        if (lastA == repA && lastB == repB)
            break;
    }

    // Assign partition
    for (int i = 0; i < nv; i++)
    {
        int vertice = vs[i];

        if (prob[i] > 0.5 + FUZZY_REGION)
        {
            part[vertice] = REPB;
        } else if (prob[i] < 0.5 - FUZZY_REGION)
        {
            part[vertice] = REPA;
        } else
        {
            part[vertice] = FUZZY;
            fuzzy++;
        }
    }
    std::cout << "Finish fuzzy decomposition, found " << fuzzy
              << " fuzzy vertices" << std::endl;
}

void GraphMesh::solve()
{
    partition.resize(num_faces);
    std::map<int, int> part;
    std::vector<int> vs;
    vs.reserve(num_faces);
    for (int i = 0; i < num_faces; i++)
        vs.push_back(i);
    meshSeg(0, 0, vs, part);
}

//Label (partition) should be id*2+1, id*2+2
void GraphMesh::meshSeg(int depth, int id, std::vector<int> &vs,
                        std::map<int, int> &part)
{
    std::cout << "Depth and id: " << depth << " " << id << std::endl;
    int fuzzy = 0;
    int repA, repB;
    float max_dist = -1.0f;
    float max_angle = -100, min_angle = 100;
    std::set<int> cur_vs(vs.begin(), vs.end());
    for (int v: vs)
    {
        for (DualEdge &de: faces[v].dedges)
        {
            if (cur_vs.find(de.face) == cur_vs.end()) continue;
            if (de.angle > max_angle) max_angle = de.angle;
            if (de.angle < min_angle) min_angle = de.angle;
        }
    }
    for (int i = 0; i < vs.size(); i++)
    {
        for (int j = 0; j < vs.size(); j++)
        {
            int m = vs[i], n = vs[j];
            if (distance[m][n] > max_dist)
            {
                max_dist = distance[m][n];
                if (m < n)
                {
                    repA = m;
                    repB = n;
                } else
                {
                    repA = n;
                    repB = m;
                }
            }
        }
    }
    std::cout << "threshold " << distance[repA][repB] << " "
              << max_angle - min_angle << std::endl;

    if (distance[repA][repB] < 30 || max_angle - min_angle < 1.1)
    {
        for (int v: vs)
        {
            partition[v] = id;
        }
        return;
    }
    fuzzyCluster(vs, part, fuzzy, repA, repB);
    if (fuzzy != 0)
        if (!buildFlowGraph(vs, part))
        {
            for (int v: vs)
            {
                partition[v] = id;
            }
            return;
        };
    if (depth >= 5)
    {
        //Assign labels
        int pa = 0, pb = 0;
        for (int v: vs)
        {
            assert(part[v] != FUZZY);
            if (part[v] == REPA)
            {
                pa++;
                partition[v] = id * 2 + 1;
            } else
            {
                pb++;
                partition[v] = id * 2 + 2;
            }
        }
        std::cout << "Finish partition, pa and pb " << pa << " " << pb << std::endl;
        return;
    }

    // Gather A and B
    std::vector<int> vs_a, vs_b;
    for (int v: vs)
    {
        assert(part[v] != FUZZY);
        if (part[v] == REPA)
        {
            vs_a.push_back(v);
        } else
        {
            vs_b.push_back(v);
        }
        part.erase(v);
    }

    std::cout << "VS, VS A, VS B " << vs.size() << " " << vs_a.size() << " "
              << vs_b.size() << std::endl;
    std::cout << "part size " << part.size() << std::endl;
    meshSeg(depth + 1, id * 2 + 1, vs_a, part);
    std::cout << "part size " << part.size() << std::endl;
    meshSeg(depth + 1, id * 2 + 2, vs_b, part);
}

// vs - all vertices used to decompose mesh
// part - corresponding partition for vs
bool GraphMesh::buildFlowGraph(std::vector<int> &vs, std::map<int, int> &part)
{
    FlowNet flowNet(num_faces);
    // Origin vertices to flow net vertices
    std::map<int, int> ori2flow;
    std::set<int> v_ca, v_cb;
    std::set<int> cur_vs(vs.begin(), vs.end());

    int src = 0, dst;
    int vcnt = 1, fuzzy = 0;
    for (int v: vs)
    {
        if (part[v] == FUZZY)
        {
            fuzzy++;
            if (ori2flow.find(v) == ori2flow.end())
            {
                ori2flow[v] = vcnt++;
            }
            int m = 0;
            for (DualEdge &de: faces[v].dedges)
            {
                if (cur_vs.find(de.face) == cur_vs.end()) continue;
                int neibor = de.face, par = part[neibor];
                if (ori2flow.find(neibor) == ori2flow.end())
                {
                    ori2flow[neibor] = vcnt++;
                }
                bool add_edge = true;
                if (par == FUZZY)
                {
                    assert(v != neibor);
                    if (neibor < v)
                    {
                        add_edge = false;
                    }
                } else if (par == REPA)
                {
                    v_ca.insert(neibor);
                } else if (par == REPB)
                {
                    v_cb.insert(neibor);
                }
                if (add_edge)
                {
                    float cap = 1.0 / (1 + de.angle_dist / avg_angle_dist);
                    int from = ori2flow[v], to = ori2flow[neibor];
                    flowNet.addEdge(from, to, cap);
                }
            }
        }
    }
    dst = vcnt;
    std::cout << "Vertices all: " << vcnt + 1 << " fuzzy: " << fuzzy
              << " VCA: " << v_ca.size() << " VCB: " << v_cb.size() << std::endl;
    if (v_ca.size() == 0 || v_cb.size() == 0) return false;
    // Add edges for src and dst
    for (int vca: v_ca)
    {
        flowNet.addEdge(0, ori2flow[vca], INF_FLOAT);
    }
    for (int vcb: v_cb)
    {
        flowNet.addEdge(dst, ori2flow[vcb], INF_FLOAT);
    }
    flowNet.num_v = vcnt + 1;
    flowNet.EK(src, dst);
    // Assign flow net result
    for (std::map<int, int>::const_iterator it = ori2flow.begin();
         it != ori2flow.end(); it++)
    {
        if (flowNet.visit[it->second])
        {
            partition[it->first] = REPA;
            part[it->first] = REPA;
        } else
        {
            partition[it->first] = REPB;
            part[it->first] = REPB;
        }
    }
    return true;
}

void GraphMesh::writeOff(const std::string &filename)
{
    std::ofstream file(filename);

    std::cout << "Writing in " << filename << std::endl;

    file << "OFF" << std::endl;
    file << vertices.size() << " " << faces.size() << " 0" << std::endl;

    for (auto &v : vertices)
        file << v.p.x[0] << " " << v.p.x[1] << " " << v.p.x[2] << std::endl;

    int i = 0;
    for (auto &f : faces)
    {
        file << "3 " << f.indices.x << " " << f.indices.y << " " << f.indices.z << " ";
        file << 60 * (partition[i] % 4 + 1) << " " << 80 * ((partition[i] + 1) % 3 + 1) << " " << 50 * ((partition[i] + 2) % 5 + 1) << std::endl;
        i++;
    }
}
