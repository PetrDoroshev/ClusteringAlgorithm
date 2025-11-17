#ifndef EUCLIDEAN_METRIC_H
#define EUCLIDEAN_METRIC_H

#include "DistanceMetric.h"

class EuclideanDistance: public DistanceMetric {

public:
    virtual double operator() (const DataPoint& point_1, const DataPoint& point_2) const override;

};


#endif