#ifndef LINKAGE_STRATEGY_H
#define LINKAGE_STRATEGY_H

#include "DistanceMatrix.h"

class LinkageStrategy {

public:

    virtual ~LinkageStrategy() = default;
    LinkageStrategy() = default;

    virtual void UpdateDistance(size_t row, size_t col, DistanceMatrix& DM) const = 0;

private:

};

#endif