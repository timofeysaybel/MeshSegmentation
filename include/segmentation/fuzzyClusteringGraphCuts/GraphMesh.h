//
// Created by timofey on 07.10.2021.
//

#ifndef MESHSEGMENTATION_GRAPHMESH_H
#define MESHSEGMENTATION_GRAPHMESH_H

#include <map>
#include "../../model/Mesh.h"

#define EPSILON 1e-12
#define INF_FLOAT 1e12
#define FUZZY_REGION 0.25
#define FUZZY -2
#define REPA -3
#define REPB -1

typedef std::pair<float, int> fipair;

/** GraphMesh extends Mesh to decompose model using Fuzzy Clustering and Graph Cuts */
class GraphMesh : public Mesh
{
public:
    std::vector<std::vector<float>> distance;
    float avg_angle_dist;

    std::vector<float> prob;
    std::vector<int> partition;

    int num_faces;
    //-----------------------------------------------------------------------------------------------------------------
    /** Returns true if Faces a and b are neighbors */
    bool isNeighFace(const Face &a, const Face &b, std::vector<int> &common);
    //-----------------------------------------------------------------------------------------------------------------
    /* Computing distance and assign probabilities.
     * Reference: Hierarchical Mesh Decomposition using Fuzzy Clustering and Cuts (3.1 and 3.2)
     * The probability that a face belongs to a certain patch depends on its distance from other faces in this patch */
    //-----------------------------------------------------------------------------------------------------------------
    /** Compute angular distance (angle between face`s normals)*/
    double angleDist(const Face &a, const Face &b, float &angle);
    /** Compute geodesic distance (shortest path between face`s dual vertices on the dual graph)*/
    double geoDist(const Face &a, const Face &b, const std::vector<int> &common);
    /** Compute distances for each face and get neighbors */
    void getDual();
    /** Compute the shortest path for each face*/
    void compDist();
    /** Dijkstra algorithm */
    void dijkstra(int src, std::vector<float> &d);
    //-----------------------------------------------------------------------------------------------------------------
    /* Mesh Segmentation.
     * Reference: Hierarchical Mesh Decomposition using Fuzzy Clustering and Cuts (3.3 and 3.4) */
    /** Generating the final decomposition */
    void meshSeg(int depth, int id, std::vector<int> &vs, std::map<int, int> &part);
    /** Initializing before mesh segmentation */
    void solve();
    /** Generating a fuzzy decomposition */
    void fuzzyCluster(std::vector<int> &vs, std::map<int, int> &part, int &fuzzy,
                      int repA, int repB);
    /** Building Flow Net(Graph) to find a boundary between the components by applying a maximum flow algorithm */
    bool buildFlowGraph(std::vector<int> &vs, std::map<int, int> &part);
    //-----------------------------------------------------------------------------------------------------------------
    // Output
    void writeOff(const std::string& filename);
};


#endif //MESHSEGMENTATION_GRAPHMESH_H
