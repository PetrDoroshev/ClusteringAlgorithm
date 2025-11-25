#include "HierarchicalClustering.h"


HClustering::HClustering(const std::vector<DataPoint>& dataset, 
                         std::shared_ptr<DistanceMetric> distance_metric, std::shared_ptr<LinkageStrategy> linkage_strategy): 
    
    dataset(dataset), distanceMetric(distance_metric), linkageStrategy(linkage_strategy) {

        
        initDistanceMatrix();

        clusterSize = std::vector<size_t> (2 * dataset.size() - 1, 0);
        for (size_t i = 0; i < dataset.size(); ++i) {
            clusterSize[i] = 1;
        }
}

void HClustering::setDataset(const std::vector<DataPoint>& dataset) {
    
    this->dataset = dataset;
    
    initDistanceMatrix();
}

void HClustering::initDistanceMatrix() {

    DM = DistanceMatrix(2 * dataset.size() - 1);

    for (size_t i = 0; i < dataset.size(); ++i) {
        for (size_t k = i + 1; k < dataset.size(); ++k){
            DM.Set(i, k, (*distanceMetric)(dataset[i], dataset[k]));
        }
    }
}

void HClustering::Fit() {
    auto L = nnChain();

    for (auto lt: L) {
        std::cout << lt.left << ", " << lt.right << ", " << lt.distance << '\n';
    }
}

std::vector<LinkageTriplet> HClustering::nnChain() {

    size_t next_label = dataset.size();

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
                            chain.size() >= 2 ? *(chain.end() - 2) : std::numeric_limits<size_t>::max()
                           );
            
            chain.push_back(nn);

            if (chain.size() >= 3 && *(chain.end() - 3) == nn) {

                auto merged_node_1 = nn;
                auto merged_node_2 = *(chain.end() - 2);

                L.push_back({merged_node_1, 
                             merged_node_2, 
                             DM.Get(merged_node_1, merged_node_2)
                            });

                removed[merged_node_1] = removed[merged_node_2] = 1;

                std::vector<size_t> na;
                for (int v: active) if (!removed[v] && v!=merged_node_1 && v!=merged_node_2) na.push_back(v);
                active.swap(na);
                
                size_t new_node = next_label++;
                active.push_back(new_node);
                clusterSize[new_node] = clusterSize[merged_node_1] + clusterSize[merged_node_2];
                          
                linkageStrategy->UpdateDistance(new_node, merged_node_1, merged_node_2, DM);
                
                break;
            }
        }
    }

    return L;
}
