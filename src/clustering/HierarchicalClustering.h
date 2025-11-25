#ifndef HIERARCHICAL_CLUSTERING_H
#define HIERARCHICAL_CLUSTERING_H

#include <vector>
#include <memory>
#include <stack>
#include <limits>
#include <iostream>
#include <iomanip>
#include "../DistanceMatrix.h"
#include "../linkage/LinkageStrategy.h"
#include "../metrics/DistanceMetric.h"
#include "../LinkageTriplet.h"

class HClustering {

public:

    HClustering(const std::vector<DataPoint>& dataset, std::shared_ptr<DistanceMetric> distance_metric, 
                std::shared_ptr<LinkageStrategy> linkage_strategy);

    void Fit();
    void setDataset(const std::vector<DataPoint>& dataset);

private:

    std::vector<size_t> clusterSize;
    std::vector<DataPoint> dataset;
    
    std::shared_ptr<DistanceMetric> distanceMetric;
    std::shared_ptr<LinkageStrategy> linkageStrategy;
 
    DistanceMatrix DM;

    void initDistanceMatrix();
    std::vector<LinkageTriplet> nnChain();
};

#endif