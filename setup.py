import pybind11
from distutils.core import setup, Extension

ext_modules = [
    Extension(
        'module', 
        ["src/main.cpp", 
         "src/dendrogram/Node.cpp",
         "src/metrics/EuclideanDistance.cpp",
         "src/linkage/WPGMA.cpp",
         "src/clustering/HierarchicalClustering.cpp",
         "src/DistanceMatrix.cpp",
         "src/DataPoint.cpp",
         "src/UnionFind.cpp"], 
        include_dirs=[pybind11.get_include()],
        language='c++',
        extra_compile_args=['-std=c++20'], 
    ),
]

setup(
    name='module',
    version='0.0.1',
    author='user',
    author_email='user@user.ru',
    description='pybind11 extension',
    ext_modules=ext_modules,
    requires=['pybind11']  
)