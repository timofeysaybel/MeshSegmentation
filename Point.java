package ModelLib;

public class Point
{
    protected double[] x = new double[3];

    protected Point() {}

    protected Point(double xx, double yy, double zz)
    {
        x = new double[]{xx, yy, zz};
    }

    protected Point sum(Point p)
    {
        Point ans = new Point();
        for (int i = 0; i < 3; i++)
            ans.x[i] = x[i] + p.x[i];
        return ans;
    }

    protected Point sub(Point p)
    {
        Point ans = new Point();
        for (int i = 0; i < 3; i++)
            ans.x[i] = x[i] - p.x[i];
        return ans;
    }

    protected Point mul(double p)
    {
        Point ans = new Point();
        for (int i = 0; i < 3; i++)
            ans.x[i] = x[i] * p;
        return ans;
    }

    protected Point div(double p)
    {
        Point ans = new Point();
        for (int i = 0; i < 3; i++)
            ans.x[i] = x[i] / p;
        return ans;
    }
}
