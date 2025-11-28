cmake_minimum_required (VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)

project ("Clustering-CLI")

add_executable(Clustering-CLI main.cpp)

target_link_libraries(Clustering-CLI PRIVATE ClusteringAlgorithm)