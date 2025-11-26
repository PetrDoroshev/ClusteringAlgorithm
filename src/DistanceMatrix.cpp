#include "DistanceMatrix.h"

DistanceMatrix::DistanceMatrix(size_t n): N(n) {

    data = std::vector<std::vector<double>>(N, std::vector<double>());

    for (auto& v: data) {
        v = std::vector<double>(N, 0.0);
    }
}

DistanceMatrix::DistanceMatrix(const std::vector<std::vector<double>>& data): N(data.size()), data(data) {

}

double DistanceMatrix::Get(size_t i, size_t k) const {
    
    if (i >= N || k >= N) {
        throw std::out_of_range("Index out of bounce");
    }   

    if (i > k) {
        std::swap(i, k);
    }

    return data[i][k];
}

void DistanceMatrix::Set(size_t i, size_t k, double val) {

     if (i >= N || k >= N) {
        throw std::out_of_range("Index out of bounce");
    }   

    if (i > k) {
        std::swap(i, k);
    }

    data[i][k] = val;
}

size_t DistanceMatrix::GetDim() const {
    return N;
}

void DistanceMatrix::Print() const {

    for (size_t i = 0; i < GetDim(); i++) {
        for (size_t k = 0; k < GetDim(); k++) {
            std::cout << std::setprecision(2) << data[i][k] << "\t";
        }
        std::cout << "\n";
    }
}
