#include <vector>
#include <random>
#include <cmath>
#include "DataPoint.h"

std::vector<DataPoint> generateData(size_t N, size_t count, double spread) {
    
    std::vector<DataPoint> pts;
    pts.reserve(count);

    std::mt19937 rng(42);
    std::normal_distribution<double> noise(0.0, spread);

    std::vector<std::vector<double>> centers = {
        std::vector<double>(N, 0.0),  
        std::vector<double>(N, 3.0),   
        std::vector<double>(N, -3.0)   
    };

    for (size_t i = 0; i < count; i++) {
        const auto& C = centers[i % centers.size()]; 
        std::vector<double> v(N);
        for (size_t d = 0; d < N; d++) {
            v[d] = C[d] + noise(rng);
        }
        pts.emplace_back(v);
    }

    return pts;
}