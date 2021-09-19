package ModelLib;

import java.util.Vector;

import static ModelLib.Utils.*;

public class Solver2 extends Solver
{
    public Solver2()
    {}

    public Solver2(Model m, Graph g, double e)
    {
        super(m, g, e);
    }

    @Override
    public void init(Vector<Integer> u, int l)
    {
        level = l;
        used = u;
        num = 2;
        reps.ensureCapacity(num);
        prob.ensureCapacity(num);
        sz = used.size();
        for (int i = 0; i < num; ++i)
            prob.get(i).ensureCapacity(sz);
        localMaxD = maxDistance();
        if (level == 0)
            Utils.globalAvgD = avgDistance();
        localAvgD = avgDistance();
        angRange = angDifference();
    }

    @Override
    public void expand()
    {
        double maxD = maxPatchDistance();
        System.out.println("minPatchD = " + maxD / globalMaxD);
        if (level > 2 || maxD / globalMaxD < Utils.TH_REP_DIST_RATIO) return;
        for (int i = 0; i < num; ++i)
        {
            Solver s = new Solver2(pModel, pGraph, eps);
            Vector<Integer> uNew = new Vector<>();
            for (int j = 0; j < sz; ++j)
                if ((pModel.faces.get(used.get(j)).label)%num == i)
                    uNew.add(used.get(j));
            s.init(uNew, level + 1);
            sons.add(s);
        }
        for (int i = 0; i < num; ++i)
        {
            System.out.println("maxD = " + sons.get(i).localMaxD / globalMaxD
                            + " angD = " + sons.get(i).angRange);

            if (sons.get(i).localMaxD / globalMaxD > TH_DIST_RATIO &&
                sons.get(i).angRange > TH_ANGLE_DIFF)
            sons.get(i).process();
        }
    }
}
