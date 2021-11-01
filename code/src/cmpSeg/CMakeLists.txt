cmake_minimum_required(VERSION 3.18)
project(graphCut)

set(CMAKE_CXX_STANDARD 14)

add_executable(cmpSeg ../model/Face.cpp ../model/Vertex.cpp ../model/Point.cpp ../model/Indices.cpp cmp.cpp ../util/Util.cpp)
