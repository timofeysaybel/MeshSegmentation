cmake_minimum_required(VERSION 3.20)
project(meshSegmentation)

set(CMAKE_CXX_STANDARD 14)

add_executable(meshSegmentation main.cpp src/model/Face.cpp include/model/Face.h src/model/Vertex.cpp include/model/Vertex.h src/io/MeshReader.cpp include/io/MeshReader.h src/model/Mesh.cpp include/model/Mesh.h src/io/MeshWriter.cpp include/io/MeshWriter.h)
