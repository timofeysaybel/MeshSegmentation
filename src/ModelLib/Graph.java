package ModelLib;

import java.util.Vector;

import static ModelLib.Utils.M_INF;
import static ModelLib.Utils.avgDa;

public class Graph
{
    protected Vector<Vector<Double>> weight;
    protected Vector<Vector<Integer>> neighbors;
    protected Vector<Integer> p;
    protected Vector<Integer> heap;
    protected int tail, sz, s;

    protected Vector<Vector<Utils.FlowEdge>> net;
    protected Vector<Integer> mask;
    protected int src, dst;
    protected Vector<Vector<Double>> dist;

    public Graph()
    {
    }

    public void build(Model model)
    {
        sz = model.faces.size();
        dist.ensureCapacity(sz);
        weight.ensureCapacity(sz);
        neighbors.ensureCapacity(sz);
        net.ensureCapacity(sz);

        for (int i = 0; i < sz; i++)
        {
            dist.get(i).ensureCapacity(sz);
            for (int j = 0; j < sz; j++)
                dist.get(i).add(j, M_INF);
            dist.get(i).add(i, 0.);
        }

        heap.ensureCapacity(sz + 2);
        p.ensureCapacity(sz);

        for (int i = 0; i < sz; i++)
        {
            for (int j = 0; j < model.faces.get(i).neighbors.size(); j++)
            {
                weight.get(i).add(model.faces.get(i).neighbors.get(j).d);
                neighbors.get(i).add(model.faces.get(i).neighbors.get(j).fid);
                Utils.FlowEdge fe = new Utils.FlowEdge();
                fe.cap = 1.0 / (1 + model.faces.get(i).neighbors.get(j).da / avgDa);
                fe.flow = 0;
                fe.id = model.faces.get(i).neighbors.get(j).fid;
                net.get(i).add(fe);
            }
        }
        src = sz;
        dst = sz + 1;
    }

    public void heapDown(int x)
    {
        while (x * 2 <= tail)
        {
            int y = x * 2;
            if (y < tail && dist.get(s).get(heap.get(y)) > dist.get(s).get(heap.get(y + 1))) y++;
            if (dist.get(s).get(heap.get(x)) > dist.get(s).get(heap.get(y)))
            {
                int tmp = heap.get(x);
                heap.set(x, heap.get(y));
                heap.set(y, tmp);
                p.set(heap.get(x), x);
                p.set(heap.get(y), y);
                x = y;
            } else break;
        }
    }

    public void heapUp(int x)
    {
        while (x > 1)
        {
            int y = x / 2;
            if (dist.get(s).get(heap.get(x)) < dist.get(s).get(heap.get(y)))
            {
                int tmp = heap.get(x);
                heap.set(x, heap.get(y));
                heap.set(y, tmp);
                p.set(heap.get(x), x);
                p.set(heap.get(y), y);
                x = y;
            } else break;
        }
    }

    public void solve()
    {
        for (s = 0; s < sz; ++s)
        {
            int count = 0;
            for (int i = 0; i < sz; ++i)
                p.set(i, 0);
            tail = 1;
            heap.set(tail, s);
            p.set(s, 1);
            while (tail > 0)
            {
                int now = heap.get(1);
                heap.set(1, heap.get(tail));
                tail--;
                count++;
                heapDown(1);
                for (int i = 0; i < neighbors.get(now).size(); ++i)
                {
                    int next = neighbors.get(now).get(i);
                    double d = weight.get(now).get(i);
                    if (dist.get(s).get(next) > dist.get(s).get(now) + d)
                    {
                        dist.get(s).set(next, dist.get(s).get(now) + d);
                        if (p.get(next) == 0) p.set(next, ++tail);
                        heap.set(p.get(next), next);
                        heapUp(p.get(next));
                        heapDown(p.get(next));
                    }
                }
            }
        }
    }

    public void initFlow(Vector<Integer> m)
    {
        mask = m;
        int netSz = 0;
        net.get(src).clear();
        for (int i = 0; i < sz; ++i)
            if (mask.get(i) == 1)
            {
                Utils.FlowEdge fe = new Utils.FlowEdge();
                fe.cap = M_INF;
                fe.flow = 0.0;
                fe.id = i;
                net.get(src).add(fe);
                netSz++;
            } else if (mask.get(i) == 2)
            {
                int last = net.get(i).size() - 1;
                if (net.get(i).get(last).id != dst)
                {
                    Utils.FlowEdge fe = new Utils.FlowEdge();
                    fe.cap = M_INF;
                    fe.flow = 0.0;
                    fe.id = dst;
                    net.get(i).add(fe);
                }
                netSz++;
            } else
            {
                if (mask.get(i) == 3) netSz++;
                int last = net.get(i).size() - 1;
                if (net.get(i).get(last).id == dst)
                    net.get(i).remove(net.get(i).size());
            }
        System.out.println("new size: " + netSz);
        mask.add(4);
        mask.add(4);
        for (int i = 0; i < sz + 2; ++i)
            if (mask.get(i) != 0)
                for (int j = 0; j < net.get(i).size(); ++j)
                    net.get(i).get(j).flow = 0.0;
    }

    public void flow(Vector<Integer> m)
    {
        Vector<Utils.FlowState> q = new Vector<>();
        Vector<Boolean> visit = new Vector<>(sz + 2);
        int head = 0, tail = 0;
        int steps = 0;
        double sumFlow = 0;
        while (true)
        {
            steps++;
            head = tail = 0;
            q.clear();
            for (int i = 0; i < visit.size(); i++)
                visit.set(i, false);
            Utils.FlowState fs = new Utils.FlowState();
            fs.id = src;
            fs.front = 0;
            fs.inc = M_INF;
            q.add(fs);
            visit.set(src, true);
            while (head <= tail && !visit.get(dst))
            {
                Utils.FlowState now = q.get(head);
                for (int i = 0; i < net.get(now.id).size(); ++i)
                {
                    int next = net.get(now.id).get(i).id;
                    if (mask.get(next) != 0) continue;
                    if (visit.get(next)) continue;
                    double inc = net.get(now.id).get(i).cap - net.get(now.id).get(i).flow;
                    if (inc < Utils.M_EPS) continue;
                    fs.id = next;
                    fs.front = head;
                    if (now.inc > inc) fs.inc = inc;
                    else fs.inc = now.inc;
                    tail++;
                    q.add(fs);
                    visit.set(next, true);
                    if (next == dst) break;
                }
                head++;
            }
            if (!visit.get(dst))
            {
                for (int i = 1; i <= tail; ++i)
                    mask.set(q.get(i).id, 1);

                System.out.println("tail = " + tail);

                for (int i = 0; i < sz; ++i)
                    if (mask.get(i) == 3) mask.set(i, 2);
                break;
            }
            double inc = q.get(tail).inc;
            sumFlow += inc;
            while (tail != 0)
            {
                int to = q.get(tail).id;
                int from = q.get(q.get(tail).front).id;

                if (inc > 1e8)
                    System.out.println(from + "->" + to);

                for (int i = 0; i < net.get(from).size(); ++i)
                    if (net.get(from).get(i).id == to)
                        net.get(from).get(i).flow += inc;
                for (int i = 0; i < net.get(to).size(); ++i)
                    if (net.get(to).get(i).id == from)
                        net.get(to).get(i).flow -= inc;
                tail = q.get(tail).front;
            }
        }
        System.out.println("steps: " + steps + ", total flow: " + sumFlow);
        m = mask;
    }
}