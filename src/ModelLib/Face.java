package ModelLib;

import java.util.Vector;

import static ModelLib.Utils.cross;
import static ModelLib.Utils.normalize;

public class Face
{
    public int[] vids = new int[3];
    public Vector<Neighbor> neighbors;
    public Point normal;
    public Point c;
    public int label;

    public Face()
    {
        label = 0;
    }

    public Face(Vertex v1, Vertex v2, Vertex v3)
    {
        c = (v1.p.sum(v2.p.sum(v3.p))).div(3.0);
        normal = cross(v2.p.sub(v1.p), v3.p.sub(v1.p));
        normalize(normal);
        //if (dot(normal, c) < 0) normal = normal * (-1.0);
        label = 0;
    }

    public Face(Face f)
    {
        vids = f.vids;
        neighbors = f.neighbors;
        normal = f.normal;
        c = f.c;
        label = f.label;
    }
}