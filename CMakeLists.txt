cmake_minimum_required(VERSION 3.18)
project(MeshSegmentation)

set(CMAKE_CXX_STANDARD 11)

add_executable(MeshSegmentation main.cpp src/Reader.cpp src/Writer.cpp src/Util.cpp src/Point.cpp src/Vertex.cpp src/Neighbor.cpp src/Face.cpp src/Edge.cpp src/Model.cpp src/Graph.cpp src/Solver.cpp src/Solver2.cpp src/SolverK.cpp)
