#include "EuclideanDistance.h"

double EuclideanDistance::operator() (const DataPoint& point_1, const DataPoint& point_2) const {
    
    if (point_1.GetDim() != point_2.GetDim())
        throw std::runtime_error("Dimension mismatch in EuclideanDistance");

    double sum = 0.0;
    size_t dim = point_1.GetDim();

    for (size_t i = 0; i < dim; i++) {
        double diff = point_1[i] - point_2[i];
        sum += diff * diff;
    }

    return sqrt(sum);

}