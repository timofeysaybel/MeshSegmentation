package ModelLib;

import java.util.Objects;
import java.util.Vector;

import static ModelLib.Utils.*;

public class Solver
{
    Model pModel;
    Graph pGraph;
    double eps;
    Vector<Integer> used;
    Vector<Integer> reps;
    Vector<Vector<Double>> prob;
    Vector<Solver> sons;
    Vector<Boolean> mask;
    int num;
    int sz;
    int level;
    double localAvgD;
    double localMaxD;
    double angRange;

    Solver()
    {
    }

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

            for (int j = 0; j < num; ++j)
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
                distNew.get(tmp).set(j, distNew.get(tmp).get(j) + pGraph.dist.get(used.get(i)).get(used.get(j)));
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
            System.out.print(mask.get(j) + " ");
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
                } else if (maxB < prob.get(j).get(i))
                {
                    maxB = prob.get(j).get(i);
                    labelB = j;
                }
            }
            if (maxA - maxB > diff)
            {
                pModel.faces.get(used.get(i)).label = types + labelA;
                c[labelA]++;
            } else
            {
                pModel.faces.get(used.get(i)).label = 1024 + (labelA * num) + labelB;
            }
        }
        System.out.print("split: " + sz + "->");
        for (int j = 0; j < num; ++j)
            System.out.print(c[j] + " ");
        System.out.println();
    }

    public void cut()
    {
        Vector<Integer> mm = new Vector<>(globalSz);
        Vector<Integer> mmNew = new Vector<>();
        for (int i = 0; i < num; ++i)
        {
            if (!mask.get(i)) continue;
            for (int j = i + 1; j < num; ++j)
            {
                if (!mask.get(j)) continue;
                for (int k = 0; k < globalSz; ++k) mm.set(k, 0);
                int nowLabelA = 1024 + i * num + j;
                int nowLabelB = 1024 + j * num + i;
                int fuzzy = 0;
                for (int k = 0; k < sz; ++k)
                    if (pModel.faces.get(used.get(k)).label == nowLabelA || pModel.faces.get(used.get(k)).label == nowLabelB)
                    {
                        fuzzy++;
                        mm.set(used.get(k), 3);
                        for (int x = 0; x < pModel.faces.get(used.get(k)).neighbors.size(); ++x)
                        {
                            int y = pModel.faces.get(used.get(k)).neighbors.get(x).fid;
                            if (pModel.faces.get(y).label == types + i) mm.set(y, 1);
                            else if (pModel.faces.get(y).label == types + j) mm.set(y, 2);
                        }
                    }
                System.out.println("fuzzy: " + fuzzy);
                pGraph.initFlow(mm);
                System.out.println("after init flow");
                pGraph.flow(mmNew);
                System.out.println("after flow");

                for (int k = 0; k < sz; ++k)
                    if (mmNew.get(used.get(k)) == 1)
                        pModel.faces.get(used.get(k)).label = types + i;
                    else if (mmNew.get(used.get(k)) == 2)
                        pModel.faces.get(used.get(k)).label = types + j;
                    else if (mmNew.get(used.get(k)) != 0)
                        System.out.println("wrong" + mmNew.get(used.get(k)));

                for (int k = 0; k < sz; ++k)
                {
                    boolean checked = false;
                    for (int x = 0; x < pModel.faces.get(used.get(k)).neighbors.size(); ++x)
                    {
                        int y = pModel.faces.get(used.get(k)).neighbors.get(x).fid;
                        if (pModel.faces.get(used.get(k)).label == pModel.faces.get(y).label)
                            checked = true;
                    }
                    if (!checked)
                        System.out.println("isolated face: " + used.get(k));
                }
            }
        }
    }

    public double avgDistance()
    {
        double sum = 0.0;
        for (int i = 0; i < sz; ++i)
            for (int j = i + 1; j < sz; ++j)
                sum += pGraph.dist.get(used.get(i)).get(used.get(j));
        sum /= sz * (sz - 1) / 2.;
        return sum;
    }

    public double angDifference()
    {
        double maxA = 0.0, minA = M_PI;
        for (int i = 0; i < sz; ++i)
        {
            int iLabel = pModel.faces.get(used.get(i)).label;
            for (int j = 0; j < pModel.faces.get(used.get(i)).neighbors.size(); ++j)
            {
                int k = pModel.faces.get(used.get(i)).neighbors.get(j).fid;
                if (k < used.get(i))
                    continue;
                int kLabel = pModel.faces.get(k).label;
                if (iLabel == kLabel)
                {
                    double ang = pModel.faces.get(used.get(i)).neighbors.get(j).a;
                    if (ang < minA) minA = ang;
                    if (ang > maxA) maxA = ang;
                }
            }
        }

        System.out.println(maxA + " " + minA);
        return maxA - minA;
    }

    public double maxDistance()
    {
        double maxD = 0.0;
        for (int i = 0; i < sz; ++i)
            for (int j = i + 1; j < sz; ++j)
                if (maxD < pGraph.dist.get(used.get(i)).get(used.get(j)))
                {
                    if (num == 2)
                    {
                        reps.set(0, used.get(i));
                        reps.set(1, used.get(j));
                    }
                    maxD = pGraph.dist.get(used.get(i)).get(used.get(j));
                }

        System.out.println("in maxDistance " + maxD);
        if (level == 0)
        {
            globalMaxD = maxD;
        }
        return maxD;
    }

    public double maxPatchDistance()
    {
        double maxD = 0;
        for(int i = 0; i < num; ++i)
            for (int j = i + 1; j < num; ++j)
            {
                if (!mask.get(j)) continue;
                if (pGraph.dist.get(reps.get(i)).get(reps.get(j)) > maxD)
                    maxD = pGraph.dist.get(reps.get(i)).get(reps.get(j));
            }

        System.out.println("maxD = " + maxD);
        System.out.println("global " + globalMaxD);
        return maxD;
    }

    public void expand()
    {
        System.out.println("!!!");
    }

    public void init(Vector<Integer> u, int l)
    {
        System.out.println("???");
    }
}