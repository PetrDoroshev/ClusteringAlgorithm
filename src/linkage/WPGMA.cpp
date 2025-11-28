#include "WPGMA.h"

void WPGMA::UpdateDistance(size_t new_node, size_t merged_node_1, size_t merged_node_2, size_t i, DistanceMatrix& DM) const {

    auto val = (DM.Get(merged_node_1, i) + DM.Get(merged_node_2, i)) / 2;
    DM.Set(new_node, i, val);
    
};