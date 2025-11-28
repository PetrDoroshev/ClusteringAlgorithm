#ifndef WPGMA_H
#define WPGMA_H

#include "LinkageStrategy.h"

class WPGMA: public LinkageStrategy {

public:

    WPGMA() = default;

    void UpdateDistance(size_t new_node, size_t merged_node_1, size_t merged_node_2, size_t i, DistanceMatrix& DM) const override;
};


#endif