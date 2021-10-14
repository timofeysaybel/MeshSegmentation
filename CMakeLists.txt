cmake_minimum_required(VERSION 3.20)
project(meshSegmentation)

set(CMAKE_CXX_STANDARD 14)

add_executable(meshSegmentation src/model/Face.cpp include/model/Face.h src/model/Vertex.cpp include/model/Vertex.h src/io/MeshReader.cpp include/io/MeshReader.h src/model/Mesh.cpp include/model/Mesh.h src/io/MeshWriter.cpp include/io/MeshWriter.h src/model/Point.cpp include/model/Point.h include/util/Util.h src/segmentation/fuzzyClusteringGraphCuts/Flow.cpp include/segmentation/fuzzyClusteringGraphCuts/Flow.h src/segmentation/fuzzyClusteringGraphCuts/GraphMesh.cpp include/segmentation/fuzzyClusteringGraphCuts/GraphMesh.h src/model/Indices.cpp include/model/Indices.h src/segmentation/fuzzyClusteringGraphCuts/main.cpp src/util/Util.cpp)
