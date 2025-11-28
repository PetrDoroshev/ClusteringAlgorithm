#ifndef HIERARCHICAL_CLUSTERING_H
#define HIERARCHICAL_CLUSTERING_H

#include <vector>
#include <memory>
#include <stack>
#include <limits>
#include <iostream>
#include <algorithm>
#include "../DistanceMatrix.h"
#include "../linkage/LinkageStrategy.h"
#include "../linkage/WPGMA.h"
#include "../metrics/DistanceMetric.h"
#include "../metrics/EuclideanDistance.h"
#include "../LinkageRow.h"
#include "../UnionFind.h"

enum class MetricType {
    EUCLIDEAN
};

enum class LinkageType {
    WPGMA
};

class HClustering {

public:

    HClustering(const std::vector<DataPoint>& dataset, MetricType distance_metric, LinkageType linkage_strategy);

    std::vector<std::tuple<size_t, size_t, double, int>> Fit();
    void setDataset(const std::vector<DataPoint>& dataset);
    DistanceMatrix getDM() const;

private:

    std::unordered_map<MetricType, std::shared_ptr<DistanceMetric>> metricMap {
        {MetricType::EUCLIDEAN, std::shared_ptr<DistanceMetric>(new EuclideanDistance)}
    };

    std::unordered_map<LinkageType, std::shared_ptr<LinkageStrategy>> linkageMap {
        {LinkageType::WPGMA, std::shared_ptr<LinkageStrategy>(new WPGMA)}
    };


    std::vector<size_t> clusterSize;
    std::vector<DataPoint> dataset;
    
    std::shared_ptr<DistanceMetric> distanceMetric;
    std::shared_ptr<LinkageStrategy> linkageStrategy;
 
    DistanceMatrix DM;

    void initDistanceMatrix();
    std::vector<LinkageRow> nnChain();
};

#endif