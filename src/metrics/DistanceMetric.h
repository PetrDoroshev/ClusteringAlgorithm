#ifndef DISTANCE_METRIC_H
#define DISTANCE_METRIC_H

#include "../DataPoint.h"

class DistanceMetric {

public:

    virtual ~DistanceMetric() = 0;
    DistanceMetric() = default;

    virtual double operator() (const DataPoint& point_1, const DataPoint& point_2) = 0;

};


#endif