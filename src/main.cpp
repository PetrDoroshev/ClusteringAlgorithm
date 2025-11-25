#include <iostream>
#include "DistanceMatrix.h"

#include <vector>
#include <random>
#include <cmath>
#include "DataPoint.h"
#include "clustering/HierarchicalClustering.h"
#include "linkage/WPGMA.h"
#include "metrics/EuclideanDistance.h"

int main () {

    size_t N = 3;
    size_t count = 10;
    double spread = 0.3;

    std::vector<DataPoint> pts;
    pts.reserve(count);

    std::mt19937 rng(42);
    std::normal_distribution<double> noise(0.0, spread);

    // Сгенерируем центры кластеров
    std::vector<std::vector<double>> centers = {
        std::vector<double>(N, 0.0),   // центр 1: (0,0,...)
        std::vector<double>(N, 3.0),   // центр 2: (3,3,...)
        std::vector<double>(N, -3.0)   // центр 3: (-3,-3,...)
    };

    for (size_t i = 0; i < count; i++) {
        const auto& C = centers[i % centers.size()]; // круглое распределение по центрам
        std::vector<double> v(N);
        for (size_t d = 0; d < N; d++) {
            v[d] = C[d] + noise(rng);
        }
        pts.emplace_back(v);
    }

    auto dm = std::shared_ptr<DistanceMetric>(new EuclideanDistance);
    auto ls = std::shared_ptr<LinkageStrategy>(new WPGMA);

    auto hc = HClustering(pts, dm, ls);
    
    hc.Fit();
    
    return 0;
}