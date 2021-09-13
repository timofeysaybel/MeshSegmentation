package ModelLib;

import java.util.Objects;
import java.util.Vector;

import static ModelLib.Utils.M_EPS;
import static ModelLib.Utils.types;

public class Solver
{
    Model pModel;
    Graph pGraph;
    double eps;
    Vector<Integer> used;
    Vector<Integer> reps;
    Vector<Vector<Double> > prob;
    Vector<Solver> sons;
    Vector<Boolean> mask;
    int num;
    int sz;
    int level;
    double localAvgD;
    double localMaxD;
    double angRange;

    Solver() {}

    Solver(Model m, Graph g, double e)
    {
        pModel = m;
        pGraph = g;
        eps = e;
    }

    public void computeProb()
    {
        for (int i = 0; i < sz; ++i)
        {
            boolean isRep = false;

            for(int j = 0; j < num; ++j)
            {
                if (used.get(i) == reps.get(j))
                {
                    isRep = true;
                    for (int k = 0; k < num; ++k)
                        prob.get(k).set(i, 0.);
                    prob.get(j).set(i, 1.);
                    break;
                }
            }

            if (isRep)
                continue;

            double sum = 0;

            for (int j = 0; j < num; ++j)
                sum += 1.0 / pGraph.dist.get(used.get(i)).get(reps.get(j));

            for (int j = 0; j < num; ++j)
                prob.get(j).set(i, 1.0 / pGraph.dist.get(used.get(i)).get(reps.get(j)) / sum);
        }
    }

    public void recomputeProb()
    {
        Vector<Integer> typeNum = new Vector<>(num);
        Vector<Vector<Double>> distNew = new Vector<>(num);
        for (int i = 0; i < num; ++i)
        {
            typeNum.set(i, 0);
            distNew.get(i).ensureCapacity(sz);
            for (int j = 0; j < sz; ++j)
                distNew.get(i).set(j, 0.);
        }
        for (int i = 0; i < sz; ++i)
        {
            int tmp = pModel.faces.get(used.get(i)).label - types;
            if (tmp < num) typeNum.set(tmp, typeNum.get(tmp) + 1);
            else continue;
            for (int j = 0; j < sz; ++j)
            {
                distNew.get(tmp).set(j,distNew.get(tmp).get(j) + pGraph.dist.get(used.get(i)).get(used.get(j)));
            }
        }
        for (int i = 0; i < num; ++i)
        {
            for (int j = 0; j < sz; ++j)
                if (typeNum.get(i) > 0)
                    distNew.get(i).set(j, distNew.get(i).get(j) / typeNum.get(i));
                else
                    distNew.get(i).set(j, Utils.M_INF);
        }
        for (int i = 0; i < sz; ++i)
        {
            double sum = 0;
            for (int j = 0; j < num; ++j)
                sum += 1.0 / (distNew.get(j).get(i) + M_EPS);
            for (int j = 0; j < num; ++j)
                prob.get(j).set(i, 1.0 / (distNew.get(j).get(i) + M_EPS) / sum);
        }
    }

    public boolean updateRep()
    {
        boolean change = false;
        for (int i = 0; i < num; ++i)
        {
            double minSum = 0.0;
            for (int j = 0; j < sz; ++j)
                minSum += prob.get(i).get(j) * pGraph.dist.get(reps.get(i)).get(used.get(j));

            for (int j = 0; j < sz; ++j)
            {
                //if (prob[i][j] < 0.5 / num) continue;
                double sum = 0;
                for (int k = 0; k < sz; ++k)
                    sum += prob.get(i).get(k) * pGraph.dist.get(used.get(j)).get(used.get(k));

                if (sum < minSum - M_EPS)
                {
                    if (!Objects.equals(reps.get(i), used.get(j))) change = true;
                    minSum = sum;
                    reps.set(i, used.get(j));
                }
            }
        }
        return change;
    }

    public void process()
    {
        int maxIter = 20;
        mask.ensureCapacity(num);
        for (int i = 0; i < maxIter; ++i)
        {
            System.out.println("iter = " + i);

            for (int j = 0; j < num; ++j)
                System.out.print(reps.get(j) + " ");
            System.out.println();
            computeProb();
            assign();
            recomputeProb();
            if (!updateRep()) break;
        }
        System.out.println("converge");
        assign();
        System.out.println("after assign 0");
        recomputeProb();
        System.out.println("after recompute");
        assign();
        System.out.println("after assign 1");
        cut();
        types += num;
        this.expand();
    }

    public void assign()
    {
        for (int i = 0; i < num; ++i)
        {
            int repeat = 0;
            for (int j = 0; j < i; ++j)
                if (reps.get(j) == reps.get(i))
                    repeat = 1;
            mask.set(i, (boolean) (1 - repeat != 0));
        }
        for (int j = 0; j < num; ++j)
            System.out.print(mask[j] + " ");
        System.out.println();

        int[] c = new int[20];

        double diff = eps * Utils.globalAvgD / localAvgD;
        if (diff > 0.2)
            diff = 0.2;

        diff = eps;
        System.out.println("diff = " + diff);
        for (int i = 0; i < sz; i++)
        {
            double maxA = -1.0, maxB = -1.0;
            int labelA = 0, labelB = 0;
            for (int j = 0; j < num; j++)
            {
                if (!mask.get(j)) continue;
                if (maxA < prob.get(j).get(i))
                {
                    maxB = maxA;
                    labelB = labelA;
                    maxA = prob.get(j).get(i);
                    labelA = j;
                }
                else if (maxB < prob.get(j).get(i))
                {
                    maxB = prob.get(j).get(i);
                    labelB = j;
                }
            }
            if (maxA - maxB > diff)
            {
                pModel.faces.get(used.get(i)).label = types+labelA;
                c[labelA]++;
            }
            else
            {
                pModel.faces.get(used.get(i)).label = 1024 + (labelA*num) + labelB;
            }
        }
        System.out.print("split: " + sz + "->");
        for(int j = 0; j < num; ++j)
            System.out.print(c[j] + " ");
        System.out.println();
    }


}
