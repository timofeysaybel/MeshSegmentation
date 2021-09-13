package ModelLib;

public class Utils
{
    protected static final double M_PI = 3.14159265358979323846;
    protected static final double M_EPS = 1e-12;
    protected static final double M_INF = 1e12;
    protected static final double TH_DIST_RATIO = 0.20;
    protected static final double TH_ANGLE_DIFF = 0.30;
    protected static final double TH_REP_DIST_RATIO = 0.1;

    protected static double avgDa;
    protected static double avgDg;

    protected static int types;
    protected static double globalMaxD;
    protected static double globalAvgD;
    protected static int globalSz;

    protected static double dot(Point p1, Point p2)
    {
        return p1.x[0] * p2.x[0] + p1.x[1] * p2.x[1] + p1.x[2] * p2.x[2];
    }

    protected static Point cross(Point p1, Point p2)
    {
        Point ans = new Point();
        ans.x[0] = p1.x[1] * p2.x[2] - p1.x[2] * p2.x[1];
        ans.x[1] = p1.x[2] * p2.x[0] - p1.x[0] * p2.x[2];
        ans.x[2] = p1.x[0] * p2.x[1] - p1.x[1] * p2.x[0];
        return ans;
    }

    protected static void normalize(Point p)
    {
        double len = Math.sqrt(dot(p, p));
        if (len < M_EPS) return;
        p.x[0] /= len;
        p.x[1] /= len;
        p.x[2] /= len;
    }

    protected static boolean isConvex(Face fa, Face fb)
    {
        return dot(fa.normal, fb.c.sub(fa.c)) < M_EPS;
    }

    protected static double faceGeoD(Face fa, Face fb, Point p0, Point p1)
    {
        Point axis = p1.sub(p0);
        Point va = fa.c.sub(p0);
        Point vb = fb.c.sub(p0);
        double lenAxis = Math.sqrt(dot(axis, axis));
        double lenA = Math.sqrt(dot(va, va));
        double lenB = Math.sqrt(dot(vb, vb));
        double angA = Math.acos(dot(va, axis) / lenA / lenAxis);
        double angB = Math.acos(dot(vb, axis) / lenB / lenAxis);
        double len = lenA * lenA + lenB * lenB - 2 * lenA * lenB * Math.cos(angA + angB);
        return len;
    }

    protected static double faceAngD(Face fa, Face fb)
    {
        double eta = 0.2;
        if (!isConvex(fa, fb))
            eta = 1.0;
        double d = dot(fa.normal, fb.normal);
        return eta * (1 - d);
    }

    protected static double faceAng(Face fa, Face fb)
    {
        double d = dot(fa.normal, fb.normal);
        return Math.acos(d);
    }

    public static class FlowEdge
    {
        int id;
        double cap;
        double flow;
    }

    public static class FlowState
    {
        int id;
        int front;
        double inc;
    }
}
