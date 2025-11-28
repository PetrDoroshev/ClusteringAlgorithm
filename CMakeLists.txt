cmake_minimum_required (VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)

project ("ClusteringAlgorithm")

add_subdirectory("./cli")

find_package(pybind11 REQUIRED)

add_library(ClusteringAlgorithm STATIC src/metrics/EuclideanDistance.cpp
                                       src/linkage/WPGMA.cpp
                                       src/clustering/HierarchicalClustering.cpp
                                       src/DistanceMatrix.cpp
                                       src/DataPoint.cpp
                                       src/UnionFind.cpp)

set_target_properties(ClusteringAlgorithm PROPERTIES POSITION_INDEPENDENT_CODE ON)


pybind11_add_module(clustering src/main.cpp)
target_link_libraries(clustering PRIVATE ClusteringAlgorithm)