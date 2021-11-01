cmake_minimum_required(VERSION 3.18)
project(regionGrowing)

set(CMAKE_CXX_STANDARD 14)

add_executable(regionGrowing
        ../../model/Face.cpp
        ../../../include/model/Face.h
        ../../model/Vertex.cpp
        ../../../include/model/Vertex.h
        ../../io/MeshReader.cpp .
        ../../../include/io/MeshReader.h
        ../../model/Mesh.cpp .
        ../../../include/model/Mesh.h
        ../../io/MeshWriter.cpp
        ../../../include/io/MeshWriter.h
        ../../model/Point.cpp
        ../../../include/model/Point.h
        ../../../include/util/Util.h
        regionGrowing.cpp
        ../../model/Indices.cpp
        ../../../include/model/Indices.h
        main.cpp
        ../../util/Util.cpp)
