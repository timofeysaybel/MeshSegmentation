package ModelLib;

public class Vertex
{
    protected Point p;

    protected Vertex() {}

    protected Vertex(double x, double y, double z)
    {
        p = new Point(x, y, z);
    }

    protected Vertex(Vertex v)
    {
        p = v.p;
    }
}
