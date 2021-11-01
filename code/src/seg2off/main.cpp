//
// Created by timofey on 25.10.2021.
//
#include <iostream>

#include "../../include/seg2off/seg2off.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        cerr << "Wrong arguments!" << endl;
        cerr << "\tArgument: <offFile> <segFile> <outFile>" << endl;
        return -1;
    }
    seg2off(argv[1], argv[2], argv[3]);
    return 0;
}

