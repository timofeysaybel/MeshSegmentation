package ModelLib;

import java.util.Vector;

import static ModelLib.Utils.*;

public class SolverK extends Solver
{
    public SolverK()
    {}

    public SolverK(Model m, Graph g, double e)
    {
        super(m, g, e);
    }

    @Override
    public void init(Vector<Integer> u, int l)
    {
        level = l;
        used = u;
        sz = used.size();
        double minSumD = M_INF;
        int choice = 0;
        for (int i = 0; i < sz; ++i)
        {
            double sumD = 0;
            for (int j = 0; j < sz; ++j)
                sumD += pGraph.dist.get(used.get(i)).get(used.get(j));
            if (sumD < minSumD)
            {
                minSumD = sumD;
                choice = used.get(i);
            }
        }
        reps.add(choice);
        Vector<Double> G = new Vector<>();
        double maxD = 0;
        for (int i = 1; i < 20; ++i)
        {
            maxD = 0;
            choice = 0;
            for (int j = 0; j < sz; ++j)
            {
                double minD = M_INF;
                for (int k = 0; k < i; ++k)
                    if (minD > pGraph.dist.get(reps.get(k)).get(used.get(j)))
                        minD = pGraph.dist.get(reps.get(k)).get(used.get(j));
                if (minD > maxD)
                {
                    maxD = minD;
                    choice = used.get(j);
                }
            }
            reps.add(choice);
            G.add(maxD);
        }
        maxD = G.get(0) - G.get(1);
        num = 2;

        for (Double aDouble : G)
            System.out.print(aDouble + " ");
        System.out.println();

        for(int i = 1; i < 18; ++i)
            if (maxD < G.get(i) - G.get(i + 1))
            {
                maxD = G.get(i) - G.get(i + 1);
                num = i + 2;
            }

        System.out.println("num = " + num);

        if (reps.size() > num)
            reps.removeElementAt(reps.size() - 1);

        prob.ensureCapacity(num);
        for (int i = 0; i < num; ++i)
            prob.get(i).ensureCapacity(sz);

        localMaxD = maxDistance();

        if (level == 0)
            globalAvgD = avgDistance();

        localAvgD = avgDistance();
        angRange = angDifference();
    }

    @Override
    public void expand()
    {
        double maxD = maxPatchDistance();
        System.out.println("minPatchD = " + maxD / globalMaxD);
        if (level > 0 || maxD / globalMaxD < TH_REP_DIST_RATIO) return;
        for (int i = 0; i < num; ++i)
        {
            Solver s = null;
            if (mask.get(i))
            {
                s = new SolverK(pModel, pGraph, eps);
                Vector<Integer> uNew = new Vector<>();
                for (int j = 0; j < sz; ++j)
                    if ((pModel.faces.get(used.get(j)).label) % num == i)
                        uNew.add(used.get(j));
                s.init(uNew, level + 1);
            }
            sons.add(s);
        }
        for (int i = 0; i < num; ++i)
        {
            if (!mask.get(i))
                continue;

            System.out.println("avgD = " + sons.get(i).localAvgD / globalAvgD
                + " angD = " + sons.get(i).angRange);
            if (sons.get(i).localAvgD / globalAvgD > TH_DIST_RATIO &&
                sons.get(i).angRange > TH_ANGLE_DIFF)
            sons.get(i).process();
        }
    }
}
