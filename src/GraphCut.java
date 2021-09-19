import ModelLib.Graph;
import ModelLib.Model;
import ModelLib.Solver;
import ModelLib.SolverK;
import io.Reader;
import io.Writer;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Vector;

import static ModelLib.Utils.*;
import static util.Utils.*;

public class GraphCut
{
    public static void main(String[] args)
    {
        if (args.length == 0)
        {
            System.out.print(ANSI_PURPLE + "Command line format: ");
            System.out.println("inputFilename(obj) outputFilename(ply)" +
                                " [binary(0=false | !0=true)] [eps(double)]" + " [delta(double)]" +
                                ANSI_RESET);
            return;
        }

        String inputFilename = args[0];
        String outputFilename = args[1];
        boolean binary = true;
        double eps = 0.04;
        double delta = 0.8;

        if (args.length > 2)
            binary = Integer.parseInt(args[2]) != 0;
        if (args.length > 3)
            eps = Double.parseDouble(args[3]);
        if (args.length > 4)
            delta = Double.parseDouble(args[4]);

        Model model = new Model();
        Graph graph = new Graph();
        try
        {
            model = Reader.read(inputFilename);
        }
        catch (FileNotFoundException e)
        {
            System.err.println(ANSI_RED + "Input file (" + inputFilename + ") not found!" + ANSI_RESET);
            return;
        }

        System.out.println(ANSI_BLUE + "Amount of faces: " + model.faces.size() + ANSI_RESET);
        System.out.println(ANSI_BLUE + "Amount of vertices: " + model.vertices.size() + ANSI_RESET);

        model.findNeighborFace(delta);
        System.out.println(ANSI_BLUE + "Distance computed" + ANSI_RESET);

        graph.build(model);
        System.out.println(ANSI_BLUE + "Graph built" + ANSI_RESET);

        graph.solve();
        System.out.println(ANSI_BLUE + "Graph solved" + ANSI_RESET);

        Vector<Integer> used = new Vector<>();
        for (int i = 0; i < model.faces.size(); i++)
            used.add(i);

        globalSz = model.faces.size();

        Solver solver = new SolverK(model, graph, eps);
        types = 0;

        solver.init(used, 0);
        System.out.println(ANSI_BLUE + "Init done" + ANSI_RESET);

        solver.process();
        System.out.println(ANSI_BLUE + "Clustering done" + ANSI_RESET);

        try
        {
            Writer.writePly(outputFilename, model);
        } catch (IOException e)
        {
            System.err.println(ANSI_RED + "Problems with output file(" + outputFilename + ")" + ANSI_RESET);
        }

        System.out.println(ANSI_BLUE + types + ANSI_RESET);
        System.out.println(ANSI_BLUE + "Output done" + ANSI_RESET);
    }
}
