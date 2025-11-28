#ifndef DISTANCE_MATRIX_H
#define DISTANCE_MATRIX_H

#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <limits>
#include <iomanip>
#include <iostream>


class DistanceMatrix {

public:

    DistanceMatrix(size_t n);
    DistanceMatrix(const std::vector<std::vector<double>>& data);
    DistanceMatrix() = default;

    double Get(size_t i, size_t k) const;
    void Set(size_t i, size_t k, double val);
    size_t GetDim() const;
    std::vector<std::vector<double>> GetData() const;

    void Print() const;

private:

    size_t N;
    std::vector<std::vector<double>> data;

};

#endif