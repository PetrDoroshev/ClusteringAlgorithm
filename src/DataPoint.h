#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <vector>
#include <cstdint>
#include <cmath>

class DataPoint {


public:
    DataPoint(const std::vector<double>& data): data(data) {};

    size_t GetDim() const;

    DataPoint operator+ (const DataPoint& rhs) const;
    DataPoint operator- (const DataPoint& rhs) const;
    DataPoint operator* (int number) const;

    double operator[] (size_t index) const;

private:
    std::vector<double> data;
};

#endif