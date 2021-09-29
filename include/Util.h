//
// Created by timofey on 28.09.2021.
//

#ifndef MESHSEGMENTATION_UTIL_H
#define MESHSEGMENTATION_UTIL_H

#include "Point.h"
#include "Face.h"

#define M_PI 3.14159265358979323846
#define M_EPS 1e-12
#define M_INF 1e12

#define TH_DIST_RATIO 0.20
#define TH_ANGLE_DIFF 0.30
#define TH_REP_DIST_RATIO 0.1

namespace util
{
    static double avg_da;
    static double avg_dg;

    static int types;
    static double globalMaxD;
    static double globalAvgD;
    static int globalSz;

    double dot(const Point& v1, const Point& v2);
    Point cross(const Point& v1, const Point& v2);
    void normalizeV(Point& v);

    bool isConvex(const Face& fa, const Face& fb);
    double faceGeoD(const Face& fa, const Face& fb, const Point& p0, const Point& p1);
    double faceAngD(const Face& fa, const Face& fb, bool inverse=false);
    double faceAng(const Face& fa, const Face& fb, bool inverse=false);

    struct FlowEdge
    {
        int id;
        double cap;
        double flow;
    };

    struct FlowState
    {
        int id;
        int front;
        double inc;
    };
}

#endif //MESHSEGMENTATION_UTIL_H
