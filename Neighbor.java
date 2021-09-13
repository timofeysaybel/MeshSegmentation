package ModelLib;

public class Neighbor
{
    protected int[] vids = new int[2];
    protected int fid;
    protected double da, dg, d, a;

    protected Neighbor() {}

    protected Neighbor(int v1, int v2, int f, double da_, double dg_, double a_)
    {
        vids[0] = v1;
        vids[1] = v2;
        fid = f;
        da = da_;
        dg = dg_;
        a = a_;
    }

    protected Neighbor(Neighbor n)
    {
        vids = n.vids;
        fid = n.fid;
        da = n.da;
        dg = n.dg;
        d = n.d;
        a = n.a;
    }

    protected void avg(double avg_da, double avg_dg, double delta)
    {
        d = (1 - delta) * da / avg_da + delta * dg / avg_dg;
        if (d < 0)
        {
            System.out.println(d + " " + da / avg_da + " " + dg / avg_dg);
        }
    }
}
