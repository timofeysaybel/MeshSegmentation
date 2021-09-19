package ModelLib;

public class Edge
{
    protected int[] vids = new int[2];
    protected int fid;

    protected Edge() {}

    public Edge(int v1, int v2, int f)
    {
        vids[0] = v1;
        vids[1] = v2;
        fid = f;
    }
}
