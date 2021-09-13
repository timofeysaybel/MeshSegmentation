package ModelLib;

import java.util.Vector;

import static ModelLib.Utils.cross;
import static ModelLib.Utils.normalize;

public class Face
{
    protected int[] vids = new int[3];
    protected Vector<Neighbor> neighbors;
    protected Point normal;
    protected Point c;
    protected int label;

    protected Face()
    {
        label = 0;
    }

    protected Face(Vertex v1, Vertex v2, Vertex v3)
    {
        c = (v1.p.sum(v2.p.sum(v3.p))).div(3.0);
        normal = cross(v2.p.sub(v1.p), v3.p.sub(v1.p));
        normalize(normal);
        //if (dot(normal, c) < 0) normal = normal * (-1.0);
        label = 0;
    }

    protected Face(Face f)
    {
        vids = f.vids;
        neighbors = f.neighbors;
        normal = f.normal;
        c = f.c;
        label = f.label;
    }
}
