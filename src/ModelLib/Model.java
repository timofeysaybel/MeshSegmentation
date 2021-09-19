package ModelLib;

import java.util.Vector;
import static ModelLib.Utils.*;

public class Model
{
    public Vector<Vertex> vertices;
    public Vector<Face> faces;
    public Vector<Edge> edges;

    public void findNeighborFace(double delta)
    {
        double sumDa = 0.0, sumDg = 0.0;
        int count = 0;
        for(int i = 0; i < edges.size(); ++i)
            for (int j = i + 1; j < edges.size(); ++j)
                if (edges.get(i).vids[0] == edges.get(j).vids[1] &&
                        edges.get(i).vids[1] == edges.get(j).vids[0])
                {
                    count++;
                    double a = Utils.faceAng(faces.get(edges.get(i).fid), faces.get(edges.get(j).fid));
                    double da = Utils.faceAngD(faces.get(edges.get(i).fid), faces.get(edges.get(j).fid));
                    double dg = Utils.faceGeoD(faces.get(edges.get(i).fid), faces.get(edges.get(j).fid),
                            vertices.get(edges.get(i).vids[0]).p, vertices.get(edges.get(i).vids[1]).p);
                    sumDa += da;
                    sumDg += dg;
                    Neighbor n1 = new Neighbor(edges.get(i).vids[0], edges.get(i).vids[1], edges.get(j).fid, da, dg, a);
                    Neighbor n2 = new Neighbor(edges.get(j).vids[0], edges.get(j).vids[1], edges.get(i).fid, da, dg, a);
                    faces.get(edges.get(i).fid).neighbors.add(n1);
                    faces.get(edges.get(j).fid).neighbors.add(n2);
                }
                else if (edges.get(i).vids[0] == edges.get(j).vids[0] &&
                        edges.get(i).vids[1] == edges.get(j).vids[1])
                {
                    count++;
                    double a = faceAng(faces.get(edges.get(i).fid), faces.get(edges.get(j).fid));
                    double da = faceAngD(faces.get(edges.get(i).fid), faces.get(edges.get(j).fid));
                    double dg = faceGeoD(faces.get(edges.get(i).fid), faces.get(edges.get(j).fid),
                            vertices.get(edges.get(i).vids[0]).p, vertices.get(edges.get(i).vids[1]).p);
                    sumDa += da;
                    sumDg += dg;
                    Neighbor n1 = new Neighbor(edges.get(i).vids[0], edges.get(i).vids[1], edges.get(j).fid, da, dg, a);
                    Neighbor n2 = new Neighbor(edges.get(j).vids[0], edges.get(j).vids[1], edges.get(i).fid, da, dg, a);
                    faces.get(edges.get(i).fid).neighbors.add(n1);
                    faces.get(edges.get(j).fid).neighbors.add(n2);
                }
        avgDa = sumDa / count;
        avgDg = sumDg / count;
        System.out.println("sumDa: " + sumDa);
        System.out.println("count: " + count);
        System.out.println("avgDa: " + avgDa);
        for(int i = 0; i < faces.size(); ++i)
        {
            for (int j = 0; j < faces.get(i).neighbors.size(); ++j)
            {
                faces.get(i).neighbors.get(j).avg(avgDa, avgDg, delta);
                if (faces.get(i).neighbors.get(j).d < -M_EPS)
                    System.out.println("negative " + i + " " + j + " " + faces.get(i).neighbors.get(j).d);
            }
        }
    }
}
