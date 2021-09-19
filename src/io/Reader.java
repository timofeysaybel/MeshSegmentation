package io;

import ModelLib.Edge;
import ModelLib.Face;
import ModelLib.Model;
import ModelLib.Vertex;

import java.io.*;
import java.util.Scanner;

public class Reader
{
    public static Model read(String filename) throws FileNotFoundException
    {
        Scanner s = new Scanner(new File(filename));
        Model model = new Model();

        String tmp;
        int fcount = 0;

        while(s.hasNextLine())
        {
            tmp = s.nextLine();
            if (tmp.isEmpty() || tmp.startsWith("#"))
                continue;

            if (tmp.startsWith("v"))
                parseV(model, tmp);

            if (tmp.startsWith("f"))
            {
                parseF(model, tmp, fcount);
                fcount++;
            }
        }

        return model;
    }

    private static void parseV(Model model, String tmp)
    {
        String[] parsedTmp = tmp.split(" ");
        double[] vs = {Double.parseDouble(parsedTmp[1]),
                Double.parseDouble(parsedTmp[2]),
                Double.parseDouble(parsedTmp[3])};

        Vertex vertex = new Vertex(vs[0], vs[1], vs[2]);

        model.vertices.add(vertex);
    }

    private static void parseF(Model model, String tmp, int fcount)
    {
        String[] parsedTmp = tmp.split(" ");
        int[] vs = {Integer.parseInt(parsedTmp[1]),
                Integer.parseInt(parsedTmp[2]),
                Integer.parseInt(parsedTmp[3])};
        vs[0]--;
        vs[1]--;
        vs[2]--;
        Face f = new Face(model.vertices.get(vs[0]), model.vertices.get(vs[1]), model.vertices.get(vs[2]));
        f.vids[0] = vs[0];
        f.vids[1] = vs[1];
        f.vids[2] = vs[2];
        model.faces.add(f);
        Edge e12 = new Edge(vs[0], vs[1], fcount);
        Edge e23 = new Edge(vs[1], vs[2], fcount);
        Edge e31 = new Edge(vs[2], vs[0], fcount);
    }
}
