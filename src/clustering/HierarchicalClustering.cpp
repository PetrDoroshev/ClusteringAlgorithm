#include "HierarchicalClustering.h"


HClustering::HClustering(const std::vector<DataPoint>& dataset, MetricType distance_metric, LinkageType linkage_strategy): 
    
    dataset(dataset), 
    distanceMetric(metricMap[distance_metric]), 
    linkageStrategy(linkageMap[linkage_strategy]) {

        
        initDistanceMatrix();

        clusterSize = std::vector<size_t> (2 * dataset.size() - 1, 0);
        for (size_t i = 0; i < dataset.size(); ++i) {
            clusterSize[i] = 1;
        }
};

void HClustering::setDataset(const std::vector<DataPoint>& dataset) {
    
    this->dataset = dataset;
    
    initDistanceMatrix();
};

DistanceMatrix HClustering::getDM() const {
    return DM;
}

void HClustering::initDistanceMatrix() {

    DM = DistanceMatrix(2 * dataset.size() - 1);

    for (size_t i = 0; i < dataset.size(); ++i) {
        for (size_t k = i + 1; k < dataset.size(); ++k){
            DM.Set(i, k, (*distanceMetric)(dataset[i], dataset[k]));
        }
    }

};

std::vector<std::tuple<size_t, size_t, double, int>> HClustering::Fit() {
    
    auto L = nnChain();

    std::stable_sort(L.begin(), L.end(), [](auto& a, auto& b) {

        return a.distance < b.distance;
        
    });

    auto U = UnionFind(L.size() + 1);

    std::vector<std::tuple<size_t, size_t, double, int>> linkage_matrix;
    linkage_matrix.reserve(L.size());

    /*
    for (auto& r: L) {
        std::cout << r.left << ", " << r.right << ", " << r.order << "\n";
    }
    */

    std::cout << "\n";
    for (auto& [a, b, d, size]: L) {

        a = U.Find(a);
        b = U.Find(b);

        if (a > b) 
            std::swap(a, b);

        linkage_matrix.push_back({a, b, d, size});
        U.Union(a, b);
    }

    for (auto&[a, b, d, size]: linkage_matrix) {
         std::cout << a << ", " << b << ", " << d << "\n";
    }

    return linkage_matrix;
};

std::vector<LinkageRow> HClustering::nnChain() {

    const size_t N = dataset.size();
    
    size_t x = 0;
    size_t y = 0;
    
    std::vector<LinkageRow> L;
    L.reserve(N - 1);
    
    std::vector<size_t> chain;
    chain.reserve(2 * N);

    std::vector<bool> active(N, false);
    for (size_t i = 0; i < N; ++i) {
        active[i] = true;
    }
    

    auto getNN = [&](size_t node, std::optional<size_t> preferable) -> size_t {

        double min_dist = std::numeric_limits<double>::max();
        size_t nn = 0;
    
        if (preferable.has_value()) {

            min_dist = DM.Get(node, preferable.value());
            nn = preferable.value();
        }
        
        for (size_t n = 0; n < DM.GetDim(); ++n) {

            if (active[n] && node != n) {
                
                auto dist = DM.Get(node, n);

                if (dist < min_dist) {
                    min_dist = dist;
                    nn = n;
                }
            }
        }

        return nn;
    };

    for (size_t k = 0; k < N - 1; ++k) {

        if (chain.empty()) {

            for (size_t i = 0; i < N; ++i) {
                
                if (active[i]) {
                    chain.push_back(i);
                    break;
                }
            }
            if (chain.empty()) break; 
        }

        while (true) {

            x = chain.back();
            y = getNN(x, chain.size() > 1 ? *(chain.end() - 2) : std::optional<size_t>());

            if (chain.size() > 1 && *(chain.end() - 2) == y) {
                break;
            }

            chain.push_back(y);
        }

        if (x > y) 
            std::swap(x, y);
        
        auto new_node = y;

        active[x] = false;
       
        L.push_back({
            x,
            y,
            DM.Get(x, y),
            clusterSize[new_node] = clusterSize[x] + clusterSize[y]
        });

        for (size_t i = 0; i < N; ++i) {
            
            if (active[i] && i != new_node) {
                
                linkageStrategy->UpdateDistance(new_node, x, y, i, DM);
            }
        }

        chain.erase(chain.end() - 2, chain.end());

    }

    return L;
};
