//
// Created by timofey on 07.10.2021.
//

#ifndef MESHSEGMENTATION_MESHWRITER_H
#define MESHSEGMENTATION_MESHWRITER_H


#include <string>
#include "../model/Mesh.h"

class MeshWriter
{
public:
    /** Save mesh in filename.off */
    static void writeOff(const std::string& filename, const Mesh& mesh);
};


#endif //MESHSEGMENTATION_MESHWRITER_H
