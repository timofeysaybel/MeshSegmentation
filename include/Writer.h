//
// Created by timofey on 28.09.2021.
//

#ifndef MESHSEGMENTATION_WRITER_H
#define MESHSEGMENTATION_WRITER_H

#include "Model.h"

class Writer
{
public:
    Writer() {}
    void write(std::string fileName, Model& model);
};

#endif //MESHSEGMENTATION_WRITER_H
