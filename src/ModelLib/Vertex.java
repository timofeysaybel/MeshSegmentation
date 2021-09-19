package ModelLib;

public class Vertex
{
    public Point p;

    protected Vertex() {}

    public Vertex(double x, double y, double z)
    {
        p = new Point(x, y, z);
    }

    protected Vertex(Vertex v)
    {
        p = v.p;
    }
}
