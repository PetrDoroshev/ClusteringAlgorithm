cmake_minimum_required (VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)

project ("CluseringAlgorithm")

add_executable(CluseringAlgorithm src/main.cpp 
                                  src/dendrogram/Dendrogram.h 
                                  src/dendrogram/Node.h
                                  src/dendrogram/Node.cpp
                                  src/metrics/DistanceMetric.h
                                  src/metrics/EuclideanDistance.h
                                  src/metrics/EuclideanDistance.cpp
                                  src/linkage/LinkageStrategy.h
                                  src/linkage/WPGMA.cpp
                                  src/clustering/HierarchicalClustering.h
                                  src/clustering/HierarchicalClustering.cpp
                                  src/DistanceMatrix.h
                                  src/DistanceMatrix.cpp
                                  src/DataPoint.h
                                  src/DataPoint.cpp)