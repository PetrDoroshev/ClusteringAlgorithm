#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>
#include <optional>

class UnionFind {

public:

    UnionFind(size_t N);

    void Union(size_t m, size_t n);
    size_t Find(size_t n);

private:

    std::vector<std::optional<size_t>> parent;
    size_t nextLabel; 
};


#endif