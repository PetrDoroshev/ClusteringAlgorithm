#include "UnionFind.h"

UnionFind::UnionFind(size_t N): nextLabel(N) {

    parent = std::vector<std::optional<size_t>>(2 * N - 1);

};

void UnionFind::Union(size_t m, size_t n) {

    parent[m] = nextLabel;
    parent[n] = nextLabel;

    nextLabel++;
};

size_t UnionFind::Find(size_t n) {
    
    while (parent[n].has_value()) {
        n = parent[n].value();
    }

    return n;
};