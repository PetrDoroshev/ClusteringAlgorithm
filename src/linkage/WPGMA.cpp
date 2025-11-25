#include "WPGMA.h"

void WPGMA::UpdateDistance(size_t new_node, size_t merged_node_1, size_t merged_node_2, DistanceMatrix& DM) const {

    for (size_t i = 0; i < new_node; ++i) {

        auto val = (DM.Get(i, merged_node_1) + DM.Get(i, merged_node_2)) / 2;
        DM.Set(i, new_node, val);
    }

};