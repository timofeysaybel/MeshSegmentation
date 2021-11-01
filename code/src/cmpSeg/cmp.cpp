//
// Created by timofey on 25.10.2021.
//
#include <iostream>
#include <set>
#include <map>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "../../include/model/Face.h"
#include "../../include/model/Vertex.h"

using namespace std;

struct RGB
{
    int r, g, b;

    RGB(int r1, int g1, int b1) : r(r1), g(g1), b(b1)
    {}

    friend bool operator==(const RGB &a, const RGB &b);

    friend bool operator<(const RGB &a, const RGB &b);
};

bool operator==(const RGB &a, const RGB &b)
{ return a.r == b.r && a.g == b.g && a.b == b.b; }

bool operator<(const RGB &a, const RGB &b)
{ return a.r < b.r && a.g < b.g && a.b < b.b; }

class Segmentation
{
    map<int, set<Face>> clusters;
    int faceAmount;

public:

    Segmentation(const string &offFile);

    double compare(const Segmentation &segmentation);
};

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

    auto it = res.begin();
    while((it = find(res.begin(), res.end(), " ")) != res.end())
    {
        res.erase(it);
    }
    return res;
}

//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    Segmentation s1(argv[1]), s2(argv[2]);

    cout << s1.compare(s2) << "%" << endl;
}

//----------------------------------------------------------------------------------------------------------------------

int getIndex(const set<RGB>& s, const RGB& rgb)
{
    int idx = 0;
    for (auto& e : s)
    {
        if (e == rgb)
            return idx;
        idx++;
    }
    return -1;
}

Segmentation::Segmentation(const string &offFile)
{
    ifstream file(offFile);

    vector<RGB> rgbs;
    set<RGB> c;

    string line;
    getline(file, line);
    while (line == "OFF" || line.empty() || line[0] == '#')
        getline(file, line);

    auto tmp = split(line, " ");
    int vSize = stoi(tmp[0]);
    int fSize = stoi(tmp[1]);
    faceAmount = fSize;

    vector <Vertex> vertices;
    for (int i = 0; i < vSize; i++)
    {
        getline(file, line);
        tmp = split(line, " ");
        vertices.emplace_back(stod(tmp[0]), stod(tmp[1]), stod(tmp[2]));
    }

    vector <Face> faces;
    for (int i = 0; i < fSize; i++)
    {
        getline(file, line);
        tmp = split(line, " ");
        if (tmp[1].empty())
            tmp.erase(tmp.begin() + 1);

        Indices indices(stoi(tmp[1]), stoi(tmp[2]), stoi(tmp[3]));
        rgbs.emplace_back(stoi(tmp[4]), stoi(tmp[5]), stoi(tmp[6]));
        c.insert(RGB(stoi(tmp[4]), stoi(tmp[5]), stoi(tmp[6])));
        faces.emplace_back(indices, vertices[indices[0]].p, vertices[indices[1]].p, vertices[indices[2]].p);
    }

    for (int i = 0; i < faces.size(); i++)
        clusters[getIndex(c, rgbs[i])].insert(faces[i]);
}

vector <int> getKeys(const map<int, set<Face>> &m)
{
    vector<int> res;

    for (auto const &k: m)
        res.push_back(k.first);

    return res;
}

int cmpCluster(const set <Face> &c1, const set <Face> &c2)
{
    int diff = 0;

    for (auto const &f: c1)
    {
        if (c2.find(f) != c2.end())
            diff++;
    }
    return diff;
}

double Segmentation::compare(const Segmentation &segmentation)
{
    int diff = 0;
    auto keys = min(getKeys(clusters), getKeys(segmentation.clusters));

    for (int i = 0; i < keys.size(); i++)
        diff += cmpCluster(clusters[keys[i]], segmentation.clusters.at(keys[i]));

    auto k1 = getKeys(clusters), k2 = getKeys(segmentation.clusters);
    if (k1 > k2)
    {
        int delta = k1.size() - k2.size();
        for (int i = k1.size() - delta - 1; i < k1.size(); i++)
            diff += clusters.at(k1[i]).size();
    }
    else
    {
        int delta = k2.size() - k1.size();
        for (int i = k2.size() - delta - 1; i < k2.size(); i++)
            diff += segmentation.clusters.at(k2[i]).size();
    }

    return 100. - (double)diff / faceAmount * 100.;
}