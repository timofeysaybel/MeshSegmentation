package io;

import ModelLib.Model;

import java.io.FileWriter;
import java.io.IOException;

public class Writer
{
    public static void writePly(String filename, Model model) throws IOException
    {
        int vs = model.vertices.size();
        int sz = model.faces.size();
        FileWriter fout = new FileWriter(filename);
        fout.write("ply\n");
        fout.write("format ascii 1.0\n");
        fout.write("element vertex " + vs + "\n");
        fout.write("property float x\n");
        fout.write("property float y\n");
        fout.write("property float z\n");
        fout.write("element face " + sz + "\n");
        fout.write("property list uchar int vertex_indices\n");
        fout.write("property uint8 red\n");
        fout.write("property uint8 green\n");
        fout.write("property uint8 blue\n");
        fout.write("end_header\n");

        for (int i = 0; i < vs; ++i)
        {
            for (int j = 0; j < 3; ++j)
                fout.write(model.vertices.get(i).p.x[j] + " ");
            fout.write("\n");
        }

        for (int i = 0; i < sz; ++i)
        {
            fout.write("3 ");
            for (int j = 0; j < 3; ++j)
                fout.write(model.faces.get(i).vids[j] + " ");
            int label = model.faces.get(i).label;
            fout.write((60 * (label % 4 + 1)) + " " + (80 * ((label + 1) % 3 + 1)) + " " + (50 * ((label + 2) % 5 + 1)) + "\n");
        }

        fout.close();
    }
}
