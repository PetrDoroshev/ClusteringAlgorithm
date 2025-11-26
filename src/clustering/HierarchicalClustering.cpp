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
        /*
        DM = DistanceMatrix(std::vector<std::vector<double>>({

            std::vector<double>({0, 17, 21, 31, 23, 0, 0, 0, 0}),
            std::vector<double>({0, 0,  30, 35, 21, 0, 0, 0, 0}),
            std::vector<double>({0, 0,  0,  28, 39, 0 ,0, 0, 0}),
            std::vector<double>({0, 0,  0,  0,  43, 0, 0, 0, 0}),
            std::vector<double>({0, 0,  0,  0,  0,  0, 0, 0, 0}),
            std::vector<double>({0, 0,  0,  0,  0,  0, 0, 0, 0}),
            std::vector<double>({0, 0,  0,  0,  0,  0, 0, 0, 0}),
            std::vector<double>({0, 0,  0,  0,  0,  0, 0, 0, 0}),
            std::vector<double>({0, 0,  0,  0,  0,  0, 0, 0, 0})
        
        }));*/
};

void HClustering::setDataset(const std::vector<DataPoint>& dataset) {
    
    this->dataset = dataset;
    
    initDistanceMatrix();
};

void HClustering::initDistanceMatrix() {

    DM = DistanceMatrix(2 * dataset.size() - 1);

    for (size_t i = 0; i < dataset.size(); ++i) {
        for (size_t k = i + 1; k < dataset.size(); ++k){
            DM.Set(i, k, (*distanceMetric)(dataset[i], dataset[k]));
        }
    }
};

void HClustering::Fit() {
    
    auto L = nnChain();

    std::vector<LinkageTriplet> nL;
    
    std::stable_sort(L.begin(), L.end(), [](auto& a, auto& b) {

        if (a.distance != b.distance) {
           return a.distance < b.distance;
        }
        return a.order < b.order;
    });

    for (auto lt: L) {
        std::cout << lt.left << ", " << lt.right << ", " << lt.distance << ", " << lt.size << '\n';
    }
    std::cout << std::endl;

    auto U = UnionFind(L.size() + 1);
    
    for (auto& [a, b, d, order, size]: L) {
        nL.push_back({U.Find(a), U.Find(b), d, order, size});
        U.Union(a, b);
    }

    for (auto lt: nL) {
        std::cout << lt.left << ", " << lt.right << ", " << lt.distance << ", " << lt.size << '\n';
    }

};

std::vector<LinkageTriplet> HClustering::nnChain() {

    size_t next_label = dataset.size();
    int order_counter = 0;

    std::vector<LinkageTriplet> L;

    std::vector<size_t> chain;
    chain.reserve(2 * dataset.size());
    
    std::vector<size_t> removed(2 * dataset.size() - 1, 0);

    std::vector<size_t> active(dataset.size(), 0);
    for (size_t i = 0; i < active.size(); ++i) {
        active[i] = i;
    }

    auto getNN = [&](size_t node, size_t preferable) -> size_t {

        double min_dist = std::numeric_limits<double>::max();
        size_t nn = 0;

        for (auto& n: active) {

            if (!removed[n] && node != n) {
                
                auto dist = DM.Get(node, n);

                if (dist < min_dist) {
                    min_dist = dist;
                    nn = n;
                }
                else if (fabs(dist - min_dist) <= 1e-12 && n == preferable) {
                    nn = preferable;
                }
            }
        }

        return nn;
    };

    while (active.size() > 1) {

        if (chain.size() <= 3) {

            chain.clear();
            chain.push_back(active[0]);
            chain.push_back(active[1]);
            
        }
        else {
            
            chain.erase(chain.end() - 3, chain.end());
        }

        while (true) {

            auto nn = getNN(chain.back(), 
                            chain.size() >= 2 ? *(chain.end() - 2) : std::numeric_limits<size_t>::max());
            
            chain.push_back(nn);

            if (chain.size() >= 3 && *(chain.end() - 3) == nn) {

                size_t new_node = next_label++;

                auto merged_node_1 = nn;
                auto merged_node_2 = *(chain.end() - 2);

                clusterSize[new_node] = clusterSize[merged_node_1] + clusterSize[merged_node_2];

                L.push_back({merged_node_1, 
                             merged_node_2, 
                             DM.Get(merged_node_1, merged_node_2),
                             order_counter++,
                             clusterSize[new_node]
                            });

                removed[merged_node_1] = removed[merged_node_2] = 1;

                std::vector<size_t> na;
                for (int v: active) if (!removed[v] && v!=merged_node_1 && v!=merged_node_2) na.push_back(v);
                active.swap(na);
                
                active.push_back(new_node);
                          
                linkageStrategy->UpdateDistance(new_node, merged_node_1, merged_node_2, DM);
                
                break;
            }
        }
    }

    return L;
};
