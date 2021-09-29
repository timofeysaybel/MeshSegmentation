//
// Created by timofey on 28.09.2021.
//

#ifndef MESHSEGMENTATION_READER_H
#define MESHSEGMENTATION_READER_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>

#include "Model.h"

class Reader
{
public:
    Reader() {}
    void read(std::string fileName, Model& model);
};

#endif //MESHSEGMENTATION_READER_H
