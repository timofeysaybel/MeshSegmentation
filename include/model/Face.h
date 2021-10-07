//
// Created by timofey on 07.10.2021.
//

#ifndef MESHSEGMENTATION_FACE_H
#define MESHSEGMENTATION_FACE_H


class Face
{
public:
    int vertices[3];

    Face() = default;

    Face(int v1, int v2, int v3);
};


#endif //MESHSEGMENTATION_FACE_H
