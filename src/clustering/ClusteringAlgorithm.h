#ifndef CLUSTERING_ALGORITHM_H
#define CLUSTERING_ALGORITHM_H

#include "../DistanceMatrix.h"
#include "../linkage/LinkageStrategy.h"
#include "../metrics/DistanceMetric.h"
#include <memory>

class ClusteringAlgorithm {

public:


private:

    std::shared_ptr<LinkageStrategy> linkage;
    std::shared_ptr<DistanceMatrix> metric;
    
    DistanceMatrix DM;

};

#endif