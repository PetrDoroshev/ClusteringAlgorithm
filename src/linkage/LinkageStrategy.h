#ifndef LINKAGE_STRATEGY_H
#define LINKAGE_STRATEGY_H

#include "../DistanceMatrix.h"

class LinkageStrategy {

public:

    virtual ~LinkageStrategy() = default;
    LinkageStrategy() = default;

    virtual void UpdateDistance(size_t new_node, size_t merged_node_1, size_t merged_node_2, size_t i, DistanceMatrix& DM) const = 0;

};

#endif