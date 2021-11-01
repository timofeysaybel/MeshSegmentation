//
// Created by timofey on 25.10.2021.
//

#ifndef MESHSEGMENTATION_REGIONGROWING_H
#define MESHSEGMENTATION_REGIONGROWING_H

#include <map>

#include "../../../include/model/Mesh.h"

class regionGrowing
{
public:

    /** Implements mesh segmentation algorithm based on region growing techniques */
    static Mesh segmentation(const Mesh& mesh);
    /** Labeling faces in mesh */
    static void decompose(Mesh &mesh);
};


#endif //MESHSEGMENTATION_REGIONGROWING_H
