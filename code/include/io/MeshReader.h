//
// Created by timofey on 07.10.2021.
//

#ifndef MESHSEGMENTATION_MESHREADER_H
#define MESHSEGMENTATION_MESHREADER_H

#include <vector>
#include <string>
#include "../model/Mesh.h"

class MeshReader
{
public:

    /** Reading <mesh>.off file and stores result in Mesh class */
    static Mesh readOff(const std::string& filename);

    static std::vector<std::string> split(const std::string& s, const std::string& delimiter);
};


#endif //MESHSEGMENTATION_MESHREADER_H
